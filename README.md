# Peter's MAC-1 Simulator (for UOG CS365)

## Installation/development Steps

Install Docker Desktop (or just Docker if on Linux)

Install Visual Studio Code

Run `git clone https://github.com/peterg98/mac-1-simulator.git` (requires git)

Install the Remote Developer Extension for Visual Studio Code

On the bottom left of screen, click the red icon and click `Remote-Containers: Open Folder in Container`

Select `Dockerfile`

## How to run

While in the container environment, run `make`. This generates `lex.yy.c` and translated binaries in `out/`.

Run `make simulate`. This runs the three `add` binaries in `out/`. You can choose which files to simulate in the `Makefile`.

(Optional) You can run `make clean` if you wish to remove all compiled files.
