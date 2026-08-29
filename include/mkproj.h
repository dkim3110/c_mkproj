#ifndef MKPROJ_H_
#define MKPROJ_H_

// == INCLUDES ============================================================
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#if !defined _WIN32
  #include <unistd.h>
#endif
// ============================================================ INCLUDES ==

// == CONSTANTS, ENUMS, & STRUCTS =========================================
#define MAX_PATH_LEN (4096)

typedef enum {
  BARE,
  DEFAULT,
  FULL,
  UNKNOWN
} project_mode_t;

typedef enum {
  README,
  LICENSE
} md_type_t;

typedef struct {
  char root[MAX_PATH_LEN];
  char bin[MAX_PATH_LEN];
  char build[MAX_PATH_LEN];
  char include[MAX_PATH_LEN];
  char lib[MAX_PATH_LEN];
  char src[MAX_PATH_LEN];
  char main_c[MAX_PATH_LEN];
  char readme[MAX_PATH_LEN];
  char lic[MAX_PATH_LEN];
  char makefile[MAX_PATH_LEN];
} project_paths_t;

typedef struct {
  int flag_count;
  project_mode_t flag;
  char *parent_directory;
} config_t;

typedef struct {
  char *name;
  project_mode_t flag;
} flag_t;
// ========================================= CONSTANTS, ENUMS, & STRUCTS ==

// == PREPROCESSORS =======================================================
#ifdef _WIN32
  #define MAKE_DIR(file_path) (mkdir(file_path))
#else
  #define MAKE_DIR(file_path) (mkdir(file_path, 0755))
#endif
// ======================================================= PREPROCESSORS ==

// == FUNCTIONS ===========================================================
  /* -- flag options ---------------------------------------------------- */
extern int option_default(const char *);
extern int option_bare(const char *);
extern int option_full(const char *);
  /* ---------------------------------------------------- flag options -- */

  /* -- populate files -------------------------------------------------- */
extern void populate_main(FILE *);
extern void populate_md(FILE *, md_type_t);
extern void populate_makefile(FILE *, project_mode_t);
  /* -------------------------------------------------- populate files -- */
// =========================================================== FUNCTIONS ==

#endif // MKPROJ_H_
