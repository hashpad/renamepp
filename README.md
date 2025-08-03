# renamepp

Fast terminal program for bulk previewing and renaming files.

---

## Overview

* **Recursively scans** a directory tree for files with the extension(s) you supply.
* **Opens** each match using `xdg-open`.
* When the viewer closes, a minimalist **ncurses** interface lets you:

  * Rename the current file.
  * Move to the next/previous file (j,k).
  * Re‑open the current file.
* It sets the cursor on rename, and after renaming sets it to next, to you can fly through your files.
---

## Quick start

```bash
# Build. You need clang++, cli11-dev, ncurses-dev
dnf install cli11-devel ncurses-devel clang++
./run.sh

# Review and rename every JPG/JPEG under ~/Pictures
./rename ~/Pictures --types jpg jpeg mp4
```

