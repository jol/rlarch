#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


// hooks.
static void * (*sys_dlopen) ( const char * file, int mode ) = NULL;
static void * (*sys_fopen)  ( const char * path, const char *mode ) = NULL;
static int    (*sys_execve) (const char *filename, char *const argv[],
                             char *const envp[]) = NULL;
 

static const char * rlarch_prefix = NULL;
static const char * prefix []= {"/usr", "/etc", "/lib", NULL};

#define DEBUG 0

// helpers
void rla_init()
{
  if(sys_dlopen == NULL) {
    sys_dlopen = (void*(*)(const char * file, int mode)) 
      dlsym( RTLD_NEXT, "dlopen" );
  }

  if(sys_fopen == NULL) {
    sys_fopen = (void*(*)(const char * file, const char* mode)) dlsym( RTLD_NEXT, "fopen" );
  }
  
  
  if(sys_execve == NULL) {
    sys_execve = (int    (*) (const char *filename, char *const argv[],
                              char *const envp[])) dlsym( RTLD_NEXT, "execve");
  }


  // check env variable.
  if(rlarch_prefix == NULL) {
    rlarch_prefix = getenv("RLARCH_PREFIX");
  }
}


int rla_file_exists (char * fileName)
{
   struct stat buf;
   int i = stat ( fileName, &buf );
     /* File found */
     if ( i == 0 )
     {
       return 1;
     }
     return 0;       
}

const char* rla_get_path(const char* filepath, char* result)
{
  int i;
  
  for(i = 0; prefix[i]; ++i) {

    // find a match with the prefix.
    if(strncmp(prefix[i], filepath, strlen(prefix[i])) == 0) {
      // if it match
      strcpy(result, rlarch_prefix);
      strcpy(result + strlen(rlarch_prefix), filepath);
      
      // check if the file exists.
      if(rla_file_exists(result)) {
        return result;
      }
    }
  }
  
  return filepath;
}


void * dlopen( const char * file, int mode )
{
  const char* to_load;
  char buf[4096];  

  if(file == NULL) {
    return NULL;
  }

  /* be sure that the env is initialized */
  rla_init();
  
  /* get file path to use */
  to_load = rla_get_path(file, buf);

  if(DEBUG) {
    printf("[rlarch::dlopen] RLARCH_PREFIX: %s; requested: %s; answer: %s\n", 
           rlarch_prefix, file, to_load); 
  }
  
  /* fall through to the system dlopen function */
  return (*sys_dlopen)( to_load, mode );
}



FILE *fopen(const char *file, const char *mode)
{
  const char* to_load;
  char buf[4096];  

  if(file == NULL) {
    return NULL;
  }

  /*
  if(strcmp(mode, "rb") != 0 && strcmp(mode, "r") != 0) {
    printf("mode: %s\n", mode);
    return sys_fopen(file, mode);
  }
  */

  /* be sure that the env is initialized */
  rla_init();
  
  /* get file path to use */
  to_load = rla_get_path(file, buf);

  if(DEBUG) {
    printf("[rlarch::fopen] RLARCH_PREFIX: %s; requested: %s; answer: %s\n", 
           rlarch_prefix, file, to_load); 
  }
  
  return sys_fopen(to_load, mode);
}



int execve(const char *file, char *const argv[],
           char *const envp[])
{
  const char* to_load;
  char buf[4096];  

  if(file == NULL) {
    return sys_execve(file, argv, envp);
  }

  /* be sure that the env is initialized */
  rla_init();
  
  /* get file path to use */
  to_load = rla_get_path(file, buf);
 
  if(DEBUG) {
    printf("[rlarch::execve] RLARCH_PREFIX: %s; requested: %s; answer: %s\n", 
           rlarch_prefix, file, to_load); 
  }
  
 
  return sys_execve(to_load, argv, envp);
}
