#ifndef MKPROJ_H_
#define MKPROJ_H_

// == INCLUDES ============================================================
#include <limits.h>
#include <stdio.h>

#ifdef _WIN32
	#include <direct.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
  #include <sys/types.h>
#endif
// ============================================================ INCLUDES ==

// == CONSTANTS, ENUMS, & STRUCTS =========================================
#define MAX_PATH_LEN (4096)

typedef enum {
	BARE,
	DEFAULT,
	FULL,
	HELP,
	PLUS,
	UNKNOWN
} project_flag_t;

typedef enum {
  GITIGNORE,
  MAKEFILE,
  MAIN_C,
  README,
  TEST_MAIN_C
} file_maker_mode_t;

typedef struct {
	char root[MAX_PATH_LEN];

	char bin[MAX_PATH_LEN];
	char build[MAX_PATH_LEN];

	char data[MAX_PATH_LEN];
	char raw[MAX_PATH_LEN];
	char interim[MAX_PATH_LEN];
	char input[MAX_PATH_LEN];
	char output[MAX_PATH_LEN];

	char docs[MAX_PATH_LEN];
	char include[MAX_PATH_LEN];
	char lib[MAX_PATH_LEN];
	char src[MAX_PATH_LEN];
	char tests[MAX_PATH_LEN];
	char test_main_c[MAX_PATH_LEN];

	char gitignore[MAX_PATH_LEN];
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
  char short_name[3];
  char *full_name;
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
extern int mkproj_generate_project(const char *, project_flag_t);
extern int mkproj_write_file(FILE *, project_flag_t, file_maker_mode_t);
// =========================================================== FUNCTIONS ==

#endif // MKPROJ_H_
