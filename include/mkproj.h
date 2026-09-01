#ifndef MKPROJ_H_
#define MKPROJ_H_

// == INCLUDES ============================================================
#include <limits.h>
#include <stdio.h>

#ifdef _WIN32
  #include <direct.h>
#else
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
} project_flag_t;

typedef struct {
  char root[MAX_PATH_LEN];

  char bin[MAX_PATH_LEN];
  char build[MAX_PATH_LEN];
  char include[MAX_PATH_LEN];
  char lib[MAX_PATH_LEN];
  char src[MAX_PATH_LEN];

  char main_c[MAX_PATH_LEN];
  char makefile[MAX_PATH_LEN];
  char readme[MAX_PATH_LEN];
} project_paths_t;

typedef struct {
  int flag_count;
  project_flag_t flag;
  char *root;
} config_t;

typedef struct {
  char *name;
  project_flag_t flag;
} flag_t;
// ========================================= CONSTANTS, ENUMS, & STRUCTS ==

// == PREPROCESSORS =======================================================
#ifdef _WIN32
  #define MAKE_DIR(file_path) (_mkdir(file_path))
  #define REMOVE_DIR(file_path) (_rmdir(file_path))
#else
  #define MAKE_DIR(file_path) (mkdir(file_path, 0755))
  #define REMOVE_DIR(file_path) (rmdir(file_path))
#endif
// ======================================================= PREPROCESSORS ==

// == FUNCTIONS ===========================================================
/* -- generate -------------------------------------------------------- */
extern int mkproj_generate_project(const char *, project_flag_t);
/* -------------------------------------------------------- generate -- */

/* -- write ----------------------------------------------------------- */
extern void mkproj_write_main_c(FILE *);
extern void mkproj_write_readme(FILE *);
extern void mkproj_write_makefile(FILE *, project_flag_t);
  /* ----------------------------------------------------------- write -- */
// =========================================================== FUNCTIONS ==

#endif // MKPROJ_H_
