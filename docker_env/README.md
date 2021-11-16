### TTSnake Docker execution environment 
### Author: @Henrique1792
### Date: 2021-11-15



1. Rationale
	After many installation workarounds pointed by colleagues, this suggestion
	was created in order to easy up everyone on system dependencies.
	It intends to allow execution without bloating host environment and solve
	compatibility issues during execution.

2. Usage
	1. Get into `docker_env` directory and build it executing:
		`docker build -t snake_env .`
	2. After it's build, run container as follows (newbie's gentle execution):
	`docker run -it --rm -v <snaskii21_path>:/data -w /data snake_env /bin/bash`
	
		Rationale:
			`-it` stands for `iterative` mode (-i keeps STDIN attached and open, while -t allocate a pseudo-tty)
			`--rm` cleans container up after execution (for small tests this flag is way useful to ignore)
			`-v HOST_DIR:CONTAINER_DIR` sets a host directory share point inside the container
			`-w WORKDIR_NAME` is your initial working directory after bring it up

3. Improvements
	1. This container gets too big if the environment suffer any modification. Maybe reconsider some steps and
		keep just execution requirements
	2. **Advices for compilation**
		You may log as super user during all process inside docker (sudo configuration isn't really recommended);
		But compilation steps which doesn't require that much permission as snakett user. Switch it running
		`su snakett` command - if root is required again, CTRL-D will get you back (or exit command)
	3. The author isn't familiar with Github actions structure, but I understand that we can create a deploy
		img from that structure, which I hope we're able to use =) .
