
trap2:     file format elf64-x86-64


Disassembly of section .init:

0000000000000668 <_init>:
 668:	48 83 ec 08          	sub    $0x8,%rsp
 66c:	48 8b 05 75 09 20 00 	mov    0x200975(%rip),%rax        # 200fe8 <__gmon_start__>
 673:	48 85 c0             	test   %rax,%rax
 676:	74 02                	je     67a <_init+0x12>
 678:	ff d0                	callq  *%rax
 67a:	48 83 c4 08          	add    $0x8,%rsp
 67e:	c3                   	retq   

Disassembly of section .plt:

0000000000000680 <.plt>:
 680:	ff 35 12 09 20 00    	pushq  0x200912(%rip)        # 200f98 <_GLOBAL_OFFSET_TABLE_+0x8>
 686:	ff 25 14 09 20 00    	jmpq   *0x200914(%rip)        # 200fa0 <_GLOBAL_OFFSET_TABLE_+0x10>
 68c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000690 <puts@plt>:
 690:	ff 25 12 09 20 00    	jmpq   *0x200912(%rip)        # 200fa8 <puts@GLIBC_2.2.5>
 696:	68 00 00 00 00       	pushq  $0x0
 69b:	e9 e0 ff ff ff       	jmpq   680 <.plt>

00000000000006a0 <__stack_chk_fail@plt>:
 6a0:	ff 25 0a 09 20 00    	jmpq   *0x20090a(%rip)        # 200fb0 <__stack_chk_fail@GLIBC_2.4>
 6a6:	68 01 00 00 00       	pushq  $0x1
 6ab:	e9 d0 ff ff ff       	jmpq   680 <.plt>

00000000000006b0 <memset@plt>:
 6b0:	ff 25 02 09 20 00    	jmpq   *0x200902(%rip)        # 200fb8 <memset@GLIBC_2.2.5>
 6b6:	68 02 00 00 00       	pushq  $0x2
 6bb:	e9 c0 ff ff ff       	jmpq   680 <.plt>

00000000000006c0 <fgets@plt>:
 6c0:	ff 25 fa 08 20 00    	jmpq   *0x2008fa(%rip)        # 200fc0 <fgets@GLIBC_2.2.5>
 6c6:	68 03 00 00 00       	pushq  $0x3
 6cb:	e9 b0 ff ff ff       	jmpq   680 <.plt>

00000000000006d0 <strtok@plt>:
 6d0:	ff 25 f2 08 20 00    	jmpq   *0x2008f2(%rip)        # 200fc8 <strtok@GLIBC_2.2.5>
 6d6:	68 04 00 00 00       	pushq  $0x4
 6db:	e9 a0 ff ff ff       	jmpq   680 <.plt>

00000000000006e0 <atoi@plt>:
 6e0:	ff 25 ea 08 20 00    	jmpq   *0x2008ea(%rip)        # 200fd0 <atoi@GLIBC_2.2.5>
 6e6:	68 05 00 00 00       	pushq  $0x5
 6eb:	e9 90 ff ff ff       	jmpq   680 <.plt>

Disassembly of section .plt.got:

00000000000006f0 <__cxa_finalize@plt>:
 6f0:	ff 25 02 09 20 00    	jmpq   *0x200902(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 6f6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

0000000000000700 <_start>:
 700:	31 ed                	xor    %ebp,%ebp
 702:	49 89 d1             	mov    %rdx,%r9
 705:	5e                   	pop    %rsi
 706:	48 89 e2             	mov    %rsp,%rdx
 709:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 70d:	50                   	push   %rax
 70e:	54                   	push   %rsp
 70f:	4c 8d 05 0a 03 00 00 	lea    0x30a(%rip),%r8        # a20 <__libc_csu_fini>
 716:	48 8d 0d 93 02 00 00 	lea    0x293(%rip),%rcx        # 9b0 <__libc_csu_init>
 71d:	48 8d 3d e6 00 00 00 	lea    0xe6(%rip),%rdi        # 80a <main>
 724:	ff 15 b6 08 20 00    	callq  *0x2008b6(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 72a:	f4                   	hlt    
 72b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000000730 <deregister_tm_clones>:
 730:	48 8d 3d e1 08 20 00 	lea    0x2008e1(%rip),%rdi        # 201018 <__TMC_END__>
 737:	55                   	push   %rbp
 738:	48 8d 05 d9 08 20 00 	lea    0x2008d9(%rip),%rax        # 201018 <__TMC_END__>
 73f:	48 39 f8             	cmp    %rdi,%rax
 742:	48 89 e5             	mov    %rsp,%rbp
 745:	74 19                	je     760 <deregister_tm_clones+0x30>
 747:	48 8b 05 8a 08 20 00 	mov    0x20088a(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 74e:	48 85 c0             	test   %rax,%rax
 751:	74 0d                	je     760 <deregister_tm_clones+0x30>
 753:	5d                   	pop    %rbp
 754:	ff e0                	jmpq   *%rax
 756:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 75d:	00 00 00 
 760:	5d                   	pop    %rbp
 761:	c3                   	retq   
 762:	0f 1f 40 00          	nopl   0x0(%rax)
 766:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 76d:	00 00 00 

0000000000000770 <register_tm_clones>:
 770:	48 8d 3d a1 08 20 00 	lea    0x2008a1(%rip),%rdi        # 201018 <__TMC_END__>
 777:	48 8d 35 9a 08 20 00 	lea    0x20089a(%rip),%rsi        # 201018 <__TMC_END__>
 77e:	55                   	push   %rbp
 77f:	48 29 fe             	sub    %rdi,%rsi
 782:	48 89 e5             	mov    %rsp,%rbp
 785:	48 c1 fe 03          	sar    $0x3,%rsi
 789:	48 89 f0             	mov    %rsi,%rax
 78c:	48 c1 e8 3f          	shr    $0x3f,%rax
 790:	48 01 c6             	add    %rax,%rsi
 793:	48 d1 fe             	sar    %rsi
 796:	74 18                	je     7b0 <register_tm_clones+0x40>
 798:	48 8b 05 51 08 20 00 	mov    0x200851(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 79f:	48 85 c0             	test   %rax,%rax
 7a2:	74 0c                	je     7b0 <register_tm_clones+0x40>
 7a4:	5d                   	pop    %rbp
 7a5:	ff e0                	jmpq   *%rax
 7a7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 7ae:	00 00 
 7b0:	5d                   	pop    %rbp
 7b1:	c3                   	retq   
 7b2:	0f 1f 40 00          	nopl   0x0(%rax)
 7b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 7bd:	00 00 00 

00000000000007c0 <__do_global_dtors_aux>:
 7c0:	80 3d 61 08 20 00 00 	cmpb   $0x0,0x200861(%rip)        # 201028 <completed.7697>
 7c7:	75 2f                	jne    7f8 <__do_global_dtors_aux+0x38>
 7c9:	48 83 3d 27 08 20 00 	cmpq   $0x0,0x200827(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 7d0:	00 
 7d1:	55                   	push   %rbp
 7d2:	48 89 e5             	mov    %rsp,%rbp
 7d5:	74 0c                	je     7e3 <__do_global_dtors_aux+0x23>
 7d7:	48 8b 3d 2a 08 20 00 	mov    0x20082a(%rip),%rdi        # 201008 <__dso_handle>
 7de:	e8 0d ff ff ff       	callq  6f0 <__cxa_finalize@plt>
 7e3:	e8 48 ff ff ff       	callq  730 <deregister_tm_clones>
 7e8:	c6 05 39 08 20 00 01 	movb   $0x1,0x200839(%rip)        # 201028 <completed.7697>
 7ef:	5d                   	pop    %rbp
 7f0:	c3                   	retq   
 7f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 7f8:	f3 c3                	repz retq 
 7fa:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000000800 <frame_dummy>:
 800:	55                   	push   %rbp
 801:	48 89 e5             	mov    %rsp,%rbp
 804:	5d                   	pop    %rbp
 805:	e9 66 ff ff ff       	jmpq   770 <register_tm_clones>

000000000000080a <main>:
 80a:	55                   	push   %rbp
 80b:	48 89 e5             	mov    %rsp,%rbp
 80e:	41 57                	push   %r15
 810:	41 56                	push   %r14
 812:	41 55                	push   %r13
 814:	41 54                	push   %r12
 816:	53                   	push   %rbx
 817:	48 83 ec 48          	sub    $0x48,%rsp
 81b:	89 7d 9c             	mov    %edi,-0x64(%rbp)
 81e:	48 89 75 90          	mov    %rsi,-0x70(%rbp)
 822:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 829:	00 00 
 82b:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
 82f:	31 c0                	xor    %eax,%eax
 831:	48 89 e0             	mov    %rsp,%rax
 834:	48 89 c3             	mov    %rax,%rbx
 837:	48 8d 3d fa 01 00 00 	lea    0x1fa(%rip),%rdi        # a38 <_IO_stdin_used+0x8>
 83e:	e8 4d fe ff ff       	callq  690 <puts@plt>
 843:	8b 05 c7 07 20 00    	mov    0x2007c7(%rip),%eax        # 201010 <MINBUF_SIZE>
 849:	48 63 d0             	movslq %eax,%rdx
 84c:	48 83 ea 01          	sub    $0x1,%rdx
 850:	48 89 55 b8          	mov    %rdx,-0x48(%rbp)
 854:	48 63 d0             	movslq %eax,%rdx
 857:	49 89 d6             	mov    %rdx,%r14
 85a:	41 bf 00 00 00 00    	mov    $0x0,%r15d
 860:	48 63 d0             	movslq %eax,%rdx
 863:	49 89 d4             	mov    %rdx,%r12
 866:	41 bd 00 00 00 00    	mov    $0x0,%r13d
 86c:	48 98                	cltq   
 86e:	ba 10 00 00 00       	mov    $0x10,%edx
 873:	48 83 ea 01          	sub    $0x1,%rdx
 877:	48 01 d0             	add    %rdx,%rax
 87a:	b9 10 00 00 00       	mov    $0x10,%ecx
 87f:	ba 00 00 00 00       	mov    $0x0,%edx
 884:	48 f7 f1             	div    %rcx
 887:	48 6b c0 10          	imul   $0x10,%rax,%rax
 88b:	48 29 c4             	sub    %rax,%rsp
 88e:	48 89 e0             	mov    %rsp,%rax
 891:	48 83 c0 00          	add    $0x0,%rax
 895:	48 89 45 c0          	mov    %rax,-0x40(%rbp)
 899:	8b 05 71 07 20 00    	mov    0x200771(%rip),%eax        # 201010 <MINBUF_SIZE>
 89f:	48 63 d0             	movslq %eax,%rdx
 8a2:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
 8a6:	be 00 00 00 00       	mov    $0x0,%esi
 8ab:	48 89 c7             	mov    %rax,%rdi
 8ae:	e8 fd fd ff ff       	callq  6b0 <memset@plt>
 8b3:	48 8b 15 66 07 20 00 	mov    0x200766(%rip),%rdx        # 201020 <stdin@@GLIBC_2.2.5>
 8ba:	8b 0d 50 07 20 00    	mov    0x200750(%rip),%ecx        # 201010 <MINBUF_SIZE>
 8c0:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
 8c4:	89 ce                	mov    %ecx,%esi
 8c6:	48 89 c7             	mov    %rax,%rdi
 8c9:	e8 f2 fd ff ff       	callq  6c0 <fgets@plt>
 8ce:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
 8d2:	48 8d 35 7f 01 00 00 	lea    0x17f(%rip),%rsi        # a58 <_IO_stdin_used+0x28>
 8d9:	48 89 c7             	mov    %rax,%rdi
 8dc:	e8 ef fd ff ff       	callq  6d0 <strtok@plt>
 8e1:	48 89 c7             	mov    %rax,%rdi
 8e4:	e8 f7 fd ff ff       	callq  6e0 <atoi@plt>
 8e9:	89 45 ac             	mov    %eax,-0x54(%rbp)
 8ec:	48 8d 35 65 01 00 00 	lea    0x165(%rip),%rsi        # a58 <_IO_stdin_used+0x28>
 8f3:	bf 00 00 00 00       	mov    $0x0,%edi
 8f8:	e8 d3 fd ff ff       	callq  6d0 <strtok@plt>
 8fd:	48 89 c7             	mov    %rax,%rdi
 900:	e8 db fd ff ff       	callq  6e0 <atoi@plt>
 905:	89 45 b0             	mov    %eax,-0x50(%rbp)
 908:	8b 55 b0             	mov    -0x50(%rbp),%edx
 90b:	8b 45 ac             	mov    -0x54(%rbp),%eax
 90e:	89 d6                	mov    %edx,%esi
 910:	89 c7                	mov    %eax,%edi
 912:	e8 4e 00 00 00       	callq  965 <trap2>
 917:	89 45 b4             	mov    %eax,-0x4c(%rbp)
 91a:	83 7d b4 01          	cmpl   $0x1,-0x4c(%rbp)
 91e:	75 0e                	jne    92e <main+0x124>
 920:	48 8d 3d 39 01 00 00 	lea    0x139(%rip),%rdi        # a60 <_IO_stdin_used+0x30>
 927:	e8 64 fd ff ff       	callq  690 <puts@plt>
 92c:	eb 0c                	jmp    93a <main+0x130>
 92e:	48 8d 3d 4d 01 00 00 	lea    0x14d(%rip),%rdi        # a82 <_IO_stdin_used+0x52>
 935:	e8 56 fd ff ff       	callq  690 <puts@plt>
 93a:	b8 00 00 00 00       	mov    $0x0,%eax
 93f:	48 89 dc             	mov    %rbx,%rsp
 942:	48 8b 4d c8          	mov    -0x38(%rbp),%rcx
 946:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 94d:	00 00 
 94f:	74 05                	je     956 <main+0x14c>
 951:	e8 4a fd ff ff       	callq  6a0 <__stack_chk_fail@plt>
 956:	48 8d 65 d8          	lea    -0x28(%rbp),%rsp
 95a:	5b                   	pop    %rbx
 95b:	41 5c                	pop    %r12
 95d:	41 5d                	pop    %r13
 95f:	41 5e                	pop    %r14
 961:	41 5f                	pop    %r15
 963:	5d                   	pop    %rbp
 964:	c3                   	retq   

0000000000000965 <trap2>:
 965:	55                   	push   %rbp
 966:	48 89 e5             	mov    %rsp,%rbp
 969:	89 7d ec             	mov    %edi,-0x14(%rbp)
 96c:	89 75 e8             	mov    %esi,-0x18(%rbp)
 96f:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%rbp)
 976:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
 97d:	eb 08                	jmp    987 <trap2+0x22>
 97f:	83 45 f8 64          	addl   $0x64,-0x8(%rbp)
 983:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
 987:	83 7d fc 09          	cmpl   $0x9,-0x4(%rbp)
 98b:	7e f2                	jle    97f <trap2+0x1a>
 98d:	8b 55 ec             	mov    -0x14(%rbp),%edx
 990:	8b 45 e8             	mov    -0x18(%rbp),%eax
 993:	01 c2                	add    %eax,%edx
 995:	8b 45 f8             	mov    -0x8(%rbp),%eax
 998:	01 c0                	add    %eax,%eax
 99a:	39 c2                	cmp    %eax,%edx
 99c:	0f 94 c0             	sete   %al
 99f:	0f b6 c0             	movzbl %al,%eax
 9a2:	5d                   	pop    %rbp
 9a3:	c3                   	retq   
 9a4:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 9ab:	00 00 00 
 9ae:	66 90                	xchg   %ax,%ax

00000000000009b0 <__libc_csu_init>:
 9b0:	41 57                	push   %r15
 9b2:	41 56                	push   %r14
 9b4:	49 89 d7             	mov    %rdx,%r15
 9b7:	41 55                	push   %r13
 9b9:	41 54                	push   %r12
 9bb:	4c 8d 25 ce 03 20 00 	lea    0x2003ce(%rip),%r12        # 200d90 <__frame_dummy_init_array_entry>
 9c2:	55                   	push   %rbp
 9c3:	48 8d 2d ce 03 20 00 	lea    0x2003ce(%rip),%rbp        # 200d98 <__do_global_dtors_aux_fini_array_entry>
 9ca:	53                   	push   %rbx
 9cb:	41 89 fd             	mov    %edi,%r13d
 9ce:	49 89 f6             	mov    %rsi,%r14
 9d1:	4c 29 e5             	sub    %r12,%rbp
 9d4:	48 83 ec 08          	sub    $0x8,%rsp
 9d8:	48 c1 fd 03          	sar    $0x3,%rbp
 9dc:	e8 87 fc ff ff       	callq  668 <_init>
 9e1:	48 85 ed             	test   %rbp,%rbp
 9e4:	74 20                	je     a06 <__libc_csu_init+0x56>
 9e6:	31 db                	xor    %ebx,%ebx
 9e8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 9ef:	00 
 9f0:	4c 89 fa             	mov    %r15,%rdx
 9f3:	4c 89 f6             	mov    %r14,%rsi
 9f6:	44 89 ef             	mov    %r13d,%edi
 9f9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 9fd:	48 83 c3 01          	add    $0x1,%rbx
 a01:	48 39 dd             	cmp    %rbx,%rbp
 a04:	75 ea                	jne    9f0 <__libc_csu_init+0x40>
 a06:	48 83 c4 08          	add    $0x8,%rsp
 a0a:	5b                   	pop    %rbx
 a0b:	5d                   	pop    %rbp
 a0c:	41 5c                	pop    %r12
 a0e:	41 5d                	pop    %r13
 a10:	41 5e                	pop    %r14
 a12:	41 5f                	pop    %r15
 a14:	c3                   	retq   
 a15:	90                   	nop
 a16:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 a1d:	00 00 00 

0000000000000a20 <__libc_csu_fini>:
 a20:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000a24 <_fini>:
 a24:	48 83 ec 08          	sub    $0x8,%rsp
 a28:	48 83 c4 08          	add    $0x8,%rsp
 a2c:	c3                   	retq   
