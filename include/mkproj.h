#ifndef MKPROJ_H_
#define MKPROJ_H_

// == INCLUDES ============================================================
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if !defined _WIN32
  #include <unistd.h>
#endif
// ============================================================ INCLUDES ==

// == CONSTANTS & ENUMS ===================================================
#define MAX_PATH_LEN (4096)

typedef enum populate_file_mode {
  DEFAULT,
  FULL
} file_mode_t;

typedef enum populate_md_type {
  README,
  LICENSE
} md_type_t;
// =================================================== CONSTANTS & ENUMS ==

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
extern void populate_makefile(FILE *, file_mode_t);
  /* -------------------------------------------------- populate files -- */
// =========================================================== FUNCTIONS ==

#endif // MKPROJ_H_
