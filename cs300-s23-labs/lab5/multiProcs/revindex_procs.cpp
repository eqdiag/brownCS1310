#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <cstring>

#include "wordindex.h"

using namespace std;

/* Helper function to convert data found in a wordindex object into a single
 * string that can be written to a pipe all at once.
 * Writes the data in the following order with the index and the phrase
 * seperated by a colon: length of a string containing the index, the index,
 * length of the phrase, the phrase.
 *
 * Arguments: a pointer to a wordindex class object, f
 * Return value: a string containing the data from f
 */
string serialize_word_index(wordindex* f) {
  string val = "";
  for (unsigned int i = 0; i < f->indexes.size(); i++) {
    string ind = to_string(f->indexes[i]);
    string ind_result = to_string(ind.length()) + ":" + ind;
    string phrase_result =
        to_string(f->phrases[i].length()) + ":" + f->phrases[i];
    val += ind_result + phrase_result;
  }
  return val;
}

/* Helper function to parse the data read from a pipe back into a wordindex
 * object
 *
 * Arguments: a pointer to a wordindex class object, ind a char*
 *            containing the data read from the pipe, buffer
 * Return value: none
 */
void deserialize_word_index(wordindex* ind, char* buffer) {
  char* tok = NULL;
  for (int i = 0; buffer[i] != '\0';) {
    tok = strtok(buffer + i, ":");  // point to length of index
    i += strlen(tok) + 1;           // i should point at index
    int len_index = atoi(tok);
    string phrase_ind =
        string(buffer + i, len_index);  // copy from i to len_index
    int newind = stoi(phrase_ind);
    ind->indexes.push_back(newind);
    i += len_index;

    tok = strtok(buffer + i, ":");
    len_index = atoi(tok);  // point to length of phrase
    i += strlen(tok) + 1;   // i should point at phrasee
    string phrase = string(buffer + i, len_index);
    ind->phrases.push_back(phrase);
    i += len_index;
  }
}

/*TODO
 * In this function, you should:
 * 1) create a wordindex object and fill it in using find_word
 * 2) write the data now contained in the wordindex object to
 *    the pipe (HINT: use serialize_word_index function so that
 *    all of the data can be written back to the parent in one
 *    call to write; and think about how you might convey the size
 *    of the data to the reader!)
 * 3) close the pipe
 *
 * Arguments: the word to search the files for, term
 *            the name of the file to search through, filename
 *            an int* representing the pipe for this process, cpipe
 * Return value: none
 */
void process_file(string term, string filename, int* cpipe) {

  //Go ahead and close read end of pipe

  wordindex file;
  file.filename = filename;
  find_word(&file,term);
 
  string str = serialize_word_index(&file);
  const char * data = str.c_str();
  int len = strlen(data) + 1;
  //printf("DATA IN %d:\n\"%s\"\nLEN: %d\n",getpid(),str.c_str(),len);

  //Write length to be expected of data
  write(cpipe[1],&len,sizeof(int));

  //Actually write data
  write(cpipe[1],data,len);

  close(cpipe[0]);
  close(cpipe[1]);

  //printf("Child %d done!\n",getpid());

}

/*TODO
 * In this function, you should:
 * 1) read the wordindex data the child process has sent through
 *    the pipe (HINT: after reading the data, use the
 *    deserialize_word_index function to convert the data back
 *    into a wordindex object)
 * 2) close the pipe
 *
 * Arguments: a pointer to a wordindex object, ind
 *            an int* representing the pipe for this process, ppipe
 * Return value: none
 */
void read_process_results(wordindex* ind, int* ppipe) {

  //Get buffer size first
  int len;
  read(ppipe[0],&len,sizeof(int));  

  if(len == 0){
    ind->indexes.clear();
    ind->phrases.clear();
    return;
  }
  
  //Then load data of that size
  char * data = new char[len];
  read(ppipe[0],data,len);

  //printf("DATA OUT %d:\n\"%s\"\nLEN: %d\n",getpid(),data,len);


  //Deseralize data into an index object
  deserialize_word_index(ind,data);  

  delete [] data;

  close(ppipe[0]);
}

/*TODO
 * Complete this function following the comments
 *
 * Arguments: the word to search for, term
 *            a vector containing the names of files
 *            to be searched, filenames
 *            an array of pipes, pipes
 *            the max number of processes that should be run at once, workers
 *            the total number of processes that need to be run, total
 */
int process_input(string term, vector<string>& filenames, int** pipes,
                  int workers, int total) {
  int pid, num_occurrences = 0;
  vector<wordindex> fls;
  int* pids = new int[workers];
  int completed = 0;  // the number of files that have been processed

  /* While the number of files that have been processed is less than the number
   * of files that need to be processed, do the steps described below:
   */
  while (completed < total) {
    // number of processes to be created in this iteration of the loop
    int num_procs = 0;
    if ((total - completed) > workers) {
      num_procs = workers;
    } else {
      num_procs = total - completed;
    }

    /* Create num_procs processes
     * For each process you should do the following:
     * 1) start a pipe for the process
     * 2) fork into a child process which runs the process_file function
     * 3) in the parent process, add the child's pid to the array of pids
     */

    for(int i = 0; i < num_procs;i++){
      if( pipe(pipes[i]) == -1) return -1;
      pid_t child = fork();
      if(child == 0){
        process_file(term,filenames[completed + i],pipes[i]);
        exit(0);
        printf("Shouldn't get here!\n");
      }else{
        //printf("Starting child %d:%d\n",i,child);
        //Parent should go ahead and close write end of pipes
        close(pipes[i][1]);
        pids[i] = child;
      }
    }

    /* Read from each pipe
     * For each processes you should do the following:
     * 1) create a wordindex object for the process (the filename can be
     *    set from the filenames array)
     * 2) use the read_process_results function to fill in the data for
     *    this file
     * 3) add the wordindex object for this file to the fls vector and
     * 4) update the total number of ocurrences accordingly
     */

    /*int proc_done = 0;
    while(proc_done != num_procs){
      if((pid = wait(NULL)) == -1) return -1;
    
      for(int i = 0; i < num_procs;i++){
        if(pids[i] == pid){
          wordindex file;
          file.filename = filenames[completed + i];
          read_process_results(&file,pipes[i]);
          file.count = file.indexes.size();
          num_occurrences += file.count;
          fls.push_back(file);
          //printf("Finishing child %d:%d\n",i,pids[i]);

        }
      }
      proc_done++;
    }*/

    for(int i = 0; i < num_procs; i++){
      wordindex file;
      file.filename = filenames[completed + i];
      //Wait for ith child
      if(waitpid(pids[i],NULL,0) != pids[i]) return -1;

      read_process_results(&file,pipes[i]);

      //Update count
      file.count = file.indexes.size();
      num_occurrences += file.count;

      //printf("Finishing child %d:%d\n",i,pids[i]);
      fls.push_back(file);
    }

    // Make sure each process created in this round has completed

    completed += num_procs;
  }

  delete[] pids;
  printOccurrences(term, num_occurrences, fls);
  return 0;
}

/* The main repl for the revindex program
 * continually reads input from stdin and determines the reverse index of that
 * input in a given directory
 *
 * Arguments: a char* containing the name of the directory, dirname
 * Return value: none
 */
void repl(char* dirname) {
  // read dir
  vector<string> filenames;
  get_files(filenames, dirname);
  int num_files = filenames.size();
  int workers = 20;

  // create pipes
  int** pipes = new int*[workers];
  for (int i = 0; i < workers; i++) {
    pipes[i] = new int[2];
  }

  printf("Enter search term: ");
  fflush(stdout);
  char buf[256];
  fgets(buf, 256, stdin);
  fflush(stdin);
  while (!feof(stdin)) {
    int len = strlen(buf) - 1;
    string term(buf, len);

    if (term.length() > 0) {
      int err = process_input(term, filenames, pipes, workers, num_files);
      if (err < 0) {
        printf("%s: %d\n", "ERROR", err);
      }
    }

    printf("\nEnter search term: ");
    fflush(stdout);

    fgets(buf, 256, stdin);
    fflush(stdin);
  }

  for (int i = 0; i < workers; i++) {
    delete[] pipes[i];
  }
  delete[] pipes;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Expected arguments: files to search through\n");
    exit(1);
  }

  char* dirname = argv[1];

  repl(dirname);

  exit(0);
}
