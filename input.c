#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<signal.h>
int end = 0;
void sig_handler(int signum){
  end = 1;
}
error_check(char *error){
  if(error < 0){
    perror(error);
    printf("\n");
  }
  exit(1);
}
int main(){
  signal(SIGUSR1,sig_handler);
  char buff[1];
  int error,i,fd,cmd,ret,errno;
  fd = open("/dev/tactsw", O_RDONLY);
  if(fd<0) error_check("open_error");
  ret = ioctl(fd,2);
  if(ret < 0)error_check("ioctl_error (fd,2)");
  for(;;){
    /*
    sleep(1);//ioctlによる情報取得
    cmd = 1; //ioctlによる情報取得
    ret = ioctl(fd,cmd);//ioctlによる情報取得
    if(ret > 0)printf("%d",ret);//ioctlによる情報取得
    if(ret < 0) error_check("ioctl_error (fd,1)");//ioctlによる情報取得
   */
    error = read(fd,buff,1);
    if(error < 0)error_check("read_error");
    error = write(1,buff,1);
    if(error < 0)error_check("write_error");

    ret = ioctl(fd,1);//ioctlによる情報取得
    if(ret < 0)error_check("ioctl_error (fd,1)");
    if(ret == 7){
      error = ioctl(fd,2);//set signal
      if(error < 0)error_check("ioctl_error (fd,2)");
      error = ioctl(fd,4);//send signal
      if(error < 0)error_check("ioctl_error (fd,4)");
    }
    
    if(ret == 6){
      error = ioctl(fd,3);//clear signal
      printf("aaa\n");
      if(error < 0)error_check("ioctl_error (fd,3)");
    }
    
    if(end){
      printf("end the program\n");
      close(fd);
      exit(1);
    }
  }
}
