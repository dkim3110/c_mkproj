#include "mkproj.h"
#include "makefile.h"

#include <stdio.h>

// == PRIMARY =============================================================
void mkproj_write_main_c(FILE *fptr) {
	fputs(
		"#include <stdio.h>\n"
		"\n"
		"int main(/* int argc, char *argv[] */) {\n"
		"\tprintf(\"Hello, World!\\n\");\n"
		"\n"
		"\treturn 0;\n"
		"}\n"
		, fptr
	);
} /* mkproj_write_main_c() */

void mkproj_write_readme(FILE *fptr) {
	fputs(
		"# Project Title\n"
		"\n"
		"Simple overview of use/purpose.\n"
		"\n"
		"## Description\n"
		"\n"
		"An in-depth paragraph about your project and overview of use.\n"
		"\n"
		"## Getting Started\n"
		"\n"
		"### Dependencies\n"
		"\n"
		"* Describe any prerequisites, libraries, OS version, etc., needed before installing program.\n"
		"\n"
		"### Installing\n"
		"\n"
		"* How/where to download your program\n"
		"* Any modifications needed to be made to files/folders\n"
		"\n"
		"### Executing program\n"
		"\n"
		"* How to run the program\n"
		"* Step-by-step bullets\n"
		"```\n"
		"code blocks for commands\n"
		"```\n"
		"\n"
		"## Help\n"
		"\n"
		"Any advise for common problems or issues.\n"
		"```\n"
		"command to run if program contains helper info\n"
		"```\n"
		"\n"
		"## Authors\n"
		"\n"
		"Contributors names and contact info\n"
		"\n"
		"## License\n"
		"\n"
		"This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details\n"
		"\n"
		"## Acknowledgments\n"
		"\n"
		"Inspiration, code snippets, etc\n"
		"* README template by [DomPizzie](https://gist.github.com/DomPizzie). You can see it [here](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)."
		, fptr
	);
} /* mkproj_write_readme() */

void mkproj_write_makefile(FILE *fptr, project_flag_t flag) {
	switch (flag) {
		case BARE:
			fputs(MAKEFILE_BARE, fptr);
			break;
		case FULL:
			fputs(MAKEFILE_FULL, fptr);
			break;
		case DEFAULT:
		default:
			fputs(MAKEFILE_DEFAULT, fptr);
			break;
	}
} /* mkproj_write_makefile() */
// ============================================================= PRIMARY ==
