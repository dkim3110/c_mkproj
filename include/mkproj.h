#ifndef MKPROJ_H_
#define MKPROJ_H_

// == INCLUDES ================================================================
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
	#include <direct.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
  #include <sys/types.h>
#endif
// ================================================================ INCLUDES ==

// == CONSTANTS, ENUMS, & STRUCTS =============================================
#define MAX_PATH_LEN (4096)

typedef enum {
	BARE = 0,
	DEFAULT = 1,
	FULL = 2,
	PLUS = 3,
	UNKNOWN = 4
} project_flag_t;

typedef enum {
	NONE = 0,
  GITIGNORE,
  CLANGD,
  MAKEFILE,
  MAIN_C,
  HEADER,
  README,
  TEST_C
} file_maker_mode_t;

#define F_BARE    (1 << BARE)
#define F_DEFAULT (1 << DEFAULT)
#define F_FULL    (1 << FULL)
#define F_PLUS    (1 << PLUS)
#define F_ALL     (F_BARE | F_DEFAULT | F_FULL | F_PLUS)
#define F_NOTBARE (F_DEFAULT | F_FULL | F_PLUS)

typedef struct {
	const char *rel_path;
	bool is_dir;
	file_maker_mode_t mode;
	int flags;
} project_node_t;

static const project_node_t PROJECT_LAYOUT[] = {
    {.rel_path = "bin",            .is_dir = true, .mode = NONE,       .flags = F_PLUS | F_FULL},
    {.rel_path = "build",          .is_dir = true, .mode = NONE,       .flags = F_NOTBARE},
    {.rel_path = "data",           .is_dir = true, .mode = NONE,       .flags = F_FULL},
    {.rel_path = "data/raw",       .is_dir = true, .mode = NONE,       .flags = F_FULL},
    {.rel_path = "data/interim",   .is_dir = true, .mode = NONE,       .flags = F_FULL},
    {.rel_path = "data/input",     .is_dir = true, .mode = NONE,       .flags = F_FULL},
    {.rel_path = "data/output",    .is_dir = true, .mode = NONE,       .flags = F_FULL},
    {.rel_path = "include",        .is_dir = true, .mode = NONE,       .flags = F_NOTBARE},
    {.rel_path = "src",            .is_dir = true, .mode = NONE,       .flags = F_NOTBARE},
    {.rel_path = "tests",          .is_dir = true, .mode = NONE,       .flags = F_PLUS | F_FULL},

    {.rel_path = ".clangd",        .is_dir = false, .mode = CLANGD,    .flags = F_ALL},
    {.rel_path = ".gitignore",     .is_dir = false, .mode = GITIGNORE, .flags = F_FULL},
    {.rel_path = "Makefile",       .is_dir = false, .mode = MAKEFILE,  .flags = F_ALL},
    {.rel_path = "README.md",      .is_dir = false, .mode = README,    .flags = F_ALL},
    {.rel_path = "tests/test.c",   .is_dir = false, .mode = TEST_C,    .flags = F_PLUS | F_FULL},

    {.rel_path = "include/main.h", .is_dir = false, .mode = HEADER,    .flags = F_NOTBARE},
    {.rel_path = "src/main.c",     .is_dir = false, .mode = MAIN_C,    .flags = F_NOTBARE},
    {.rel_path = "main.h",         .is_dir = false, .mode = HEADER,    .flags = F_BARE},
    {.rel_path = "main.c",         .is_dir = false, .mode = MAIN_C,    .flags = F_BARE}
};

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
// ============================================= CONSTANTS, ENUMS, & STRUCTS ==

// == PREPROCESSORS ===========================================================
#ifdef _WIN32
	#define MAKE_DIR(file_path) (_mkdir(file_path))
	#define REMOVE_DIR(file_path) (_rmdir(file_path))
#else
	#define MAKE_DIR(file_path) (mkdir(file_path, 0777))
	#define REMOVE_DIR(file_path) (rmdir(file_path))
#endif
// =========================================================== PREPROCESSORS ==

// == FUNCTIONS ===============================================================
extern int mkproj_generate_project(const char *, project_flag_t);
extern int mkproj_write_file(FILE *, project_flag_t, file_maker_mode_t);
// =============================================================== FUNCTIONS ==

#endif // MKPROJ_H_
