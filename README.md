# TexTron Snake 🐍

TexTron Snake is a very simple ASCII snake game tributed to the classic
Tron arcade. It was meant to be developed as a collaborative programming
exercise in a course on Open Source Systems taught to undergraduate CS
students.

<p align="center">
    <img src="./scenes/opening.gif" width=50% height=50%>
</p>

## SETUP

### Dependencies


If you have obtained the project source from the __version control
repository__, you'll need to have GNU Build System (Autotools) installed. 

In case you do not hear any music, make sure you have mpg123 installed in your current machine executing the code.

### Debian/Ubuntu based platforms:

```
sudo apt install automake autoconf
```

### [Homebrew](https://brew.sh/) (macOS/Linux):

```
brew install autoconf automake
```

### Arch Linux/Manjaro based platforms:

```
sudo pacman -S automake autoconf
```

Other missing dependencies will be indicated by the configuration script,
e.g.: if you don't have `libncurses` installed, it'll complain about it and you
may install it.

### Debian/Ubuntu based platforms:

```
sudo apt install libncurses5-dev
```

### [Homebrew](https://brew.sh/) (macOS/Linux):

```
brew install ncurses
```

### Arch Linux/Manjaro based platforms:

```
sudo pacman -S ncurses
```

It is also required for your system to have support for POSIX threads.

### Installation

If you have obtained the project source from the __version control repository__,
execute the script

 ```
 ./autogen.sh
 ```

to bootstrap the build configuration script `configure`.

If you're using Ubuntu on WSL, maybe you can face the following problem 
`-bash: ./autogen.sh: /bin/bash^M: bad interpreter: No such file or directory`

Execute the following commands to solve the problem

```
$ sed -i -e 's/\r$//' autogen.sh
$ ./autogen.sh
```

On the other hand, if you have obtained the software from a __distribution
repository__, usually as a tarball, you should already have the script
`configure`, which performs a series of tests to collect data about the build
platform.

Either way, locate the file in the root of source directory and execute it:

```
 ./configure
```

Again, if you're using Ubuntu on WSL, maybe you can face the following problem 
`configure: error: cannot run /bin/bash build-aux/config.sub`

Execute the following commands to solve the problem

```
$ autoreconf -ivf
$ dos2unix configure.ac
$ autoconf
$ ./configure
```

If it complains about missing pieces of software, install them as needed.


Finally, build the software and install it either globally or locally.

To install the program under the system path --- usually placing the binary
in `/usr/bin` and the data files in `/usr/share` ---, run:

```
make
sudo make install
```

Optionally, if you wish to install the software under a different location,
 for instance, in `/tmp/foo`, execute:

```
./configure --prefix=/tmp/foo
make
make install
```

This shall install the software locally, with the binary in `/tmp/foo/bin`
and the data files in `/tmp/share`.

For more detailed instructions, please, refer to the
[installation guide](./INSTALL).

An alternative option to install the dependencies and the repository is to use the provided automated scripts, such as `install_all.sh` or `install_all.py` scripts.

For `install_all.sh`, execute the following command on the terminal:

``sudo ./install_all.sh```

For `install_all.py`, it's required to use Python 3.x. The usage is:

```python ./install_all.py [--password SUDO_PASSWORD]```

The password argument is optional. It's required only if your system is unable to install the packages and the repository without root privileges. In this case, type your password on the terminal as a command-line argument (for example, supposing a password `xyz`):

```python ./install_all.py --password xyz```


## Docker environment execution

For instructions on how to compile and execute using Docker, checkout the [dedicated document](/docker_env/README.md).



## USAGE

### Opening the game

After following the installation process, execute the following command to open the game:

```
ttsnake
```

If, for some reason, the command returns a message asking for a minimum terminal size to play the game, try running the command with the terminal in fullscreen mode.

### Extra options 

```
 Usage:  ttsnake [options]

         Options
		
         -d, --data      Selects a non-default data path to game's assets and "share" folder
         -h, --help      Displays this information message
         -v, --version   Outputs the program version
```

### Playing the game

The game takes place on a rectangular arena where a snake is continuously moving (that is,
it never stops) in one of the four directions: left, right, up, and down.
As the snake moves, it loses energy and if all of it is exhausted, the
snake dies. To recover energy, the snake needs to eat pieces of food that
are constantly replaced at random positions.

Be careful, though. The arena borders are electrified and will kill the snake
if touched. Moreover, mind that the snake is poisonous and it will also die if
it accidentally bites itself, i.e. if the snake's head crosses its own body (yes,
this is weird for snakes, but this is a Tron Snake).

The game score is the count of eaten blocks until the game is over.

 ### Controls:
	WASD/HJKL/ARROWS to control the snake
	+ to increase the game speed
	- to decrease the game speed
	Q to quit the game
	Space to skip the loading animation
	R at game over screen to restart the game
	m/n on menu to change the number of walls
	P to pause the game

## Contributing

If you wish to contribute to the project, please, __do__ read the
[contribution guide](docs/CONTRIBUTING.md) for important information and
guidelines.
