#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

extern void * __libc_dlopen(const char*, int);

void * dlopen( const char * file, int mode )
{
  static void * (*sys_dlopen)( const char * file, int mode ) = NULL;
  static const char * prefix []= {"/usr", "/etc", "/lib", NULL};
  static const char * rlarch_prefix = NULL;
  int i;
  const char* to_load = file;
  char buf[4096];

  if(sys_dlopen == NULL) {
    sys_dlopen = (void*(*)(const char * file, int mode)) dlsym( RTLD_NEXT, "dlopen" );
  }

  if(rlarch_prefix == NULL) {
    rlarch_prefix = getenv("RLARCH_PREFIX");
  }

  if(file == NULL) {
    return NULL;
  }

  printf("[start dlopen]going to load: %s; RLARCH_PREFIX: %s\n", file, rlarch_prefix);
  for(i = 0; prefix[i]; ++i) {
    if(strcmp(prefix[i], file) == 0) {
      strcpy(buf, rlarch_prefix);
      strcpy(buf + strlen(rlarch_prefix), file);
      to_load = buf;
      break;
    }
  }
  

  /* look up the pointer to the system's dlopen function */

  printf("[finish dlopen]going to load: %s\n", to_load);
  

  /* fall through to the system dlopen function */
  return (*sys_dlopen)( to_load, mode );

}



FILE *fopen(const char *path, const char *mode)
{
  static void * (*sys_fopen)( const char * path, const char *mode ) = NULL;

  printf("[start fopen] going to open file: %s\n", path);

  if(sys_fopen == NULL) {
    sys_fopen = (void*(*)(const char * file, const char* mode)) dlsym( RTLD_NEXT, "fopen" );
  }

  printf("[finish fopen]\n");

  return sys_fopen(path, mode);
}
