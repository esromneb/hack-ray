
# Build Quirks

Due to how emscripted operates, we pass it a shell file.  In the shell file is the line `{{{ SCRIPT }}}`.

* In dev and gh pages mode, we let this all happen.  emcc writes code into ray.html
* In module (the dist folder for npm).  We don't use ray.html at all as the user will use their own.  We capture the contents of this script


# out/rayInterface.js
We have another shell, "extract_script_template.html" that is 100% empty except for the script tag.

In the makefile we build this, and then use cut to strip the html tags off, so we can use it as js
