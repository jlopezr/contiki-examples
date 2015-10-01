#include "contiki.h"
#include "cfs/cfs.h"
#include <stdio.h>

PROCESS(cfs_process, "Test CFS process");
AUTOSTART_PROCESSES(&cfs_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cfs_process, ev, data)
{
  PROCESS_BEGIN();

  int fd,i,len;
  char buf[50];
  fd = cfs_open("result.log", CFS_WRITE | CFS_APPEND);

  for(i=0;i<10;i++) {
    len = sprintf(buf,"LINE %d\n",i);
    if(cfs_write(fd, buf, len) < len) {
        printf("Failed to write!");
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
