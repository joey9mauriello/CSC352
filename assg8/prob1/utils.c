#include "utils.h"

/*
* File: utils.c
* Author: Joey Mauriello
* Purpose: This program defines all functions used for mymake.c
*/

target *head = NULL;

/*
* processMakeFile(fp) -- reads fp and creates a dependency graph based on make commands
*/
int processMakeFile(FILE *fp) {
	char *line = NULL;
	size_t len = 0;
	char *parentPtr = NULL;
	char *childrenPtr = NULL;
	int err = 0;
	
	while (getline(&line, &len, fp) > 0) {
		if (strcmp(line, "\n") != 0) {
			char *colon = strchr(line, ':');
			if (colon != NULL) {
				
				childrenPtr = colon+1;
				*colon = '\0';
				parentPtr = strdup(line);
				if (!parentPtr) {
					free(line);
					fprintf(stderr, "No target specified\n");
					return 1;
				}
				err = addNodes(parentPtr, childrenPtr);
				if (err == 1) {
					free(parentPtr);
					free(line);
					return 1;
				}
			}
			else {
				err = addCmd(line);
				if (err == 1) {
					free(line);
					return 1;
				}
			}
			
			if (childrenPtr) {
				childrenPtr = NULL;
			}
			if (parentPtr) {
				free(parentPtr);
				parentPtr = NULL;
			}
		}
		
		
	}
	

	free(parentPtr);
	free(childrenPtr);
	free(line);
	
	return 0;
}

/*
* traverseGraph(fp) -- Gets the desired target and prints the dependencies in post order traversal
*/
int traverseGraph(char *fp) {
	
	target *myTarget = getTarget(fp);
	if (!myTarget) {
		fprintf(stderr, "Invalid target\n");
		return 1;
	}
	
	postOrder(myTarget);
	return 0;
}

/*
* addNodes(parent, children) -- Adds the target parent to the dependency graph with children as its dependencies
*/
int addNodes(char *parent, char *children) {
	char str[65];
	int n;
	char *ptr = children;
	stripWhitespace(parent);
	
	
	target *new = createTarget(parent);
	if (targetExists(new)) {
		free(new -> name);
		free(new);
		fprintf(stderr, "Target already exists\n");
		return 1;
	}
	
	children[strlen(children)-1] = '\0';
	
	while (sscanf(ptr, "%64s%n", str, &n) > 0) {
		stripWhitespace(str);
		addDependency(str, new);
		ptr += n;
	}
	
	if (!head) {
		head = new;
	}
	else {
		target *curr = head;
		while (curr -> next) {
			curr = curr -> next;
		}
		curr -> next = new;
	}
	
	return 0;
	
}

/*
* addCmd(cmd) -- Adds cmd to the newest target in the dependency graph
*/
int addCmd(char *cmd) {
	
	if (cmd[0] == '\t') {
		memmove(cmd + 1, cmd, strlen(cmd) - 1);
		cmd[0] = ' ';
		cmd[1] = ' ';
		if (cmd[strlen(cmd)-1] == '\n') {
			cmd[strlen(cmd)-1] = '\0';
		}
		if (!head) {
			fprintf(stderr, "No target for command\n");
			return 1;
		}
		target *curr = head;
		while (curr -> next) {
			curr = curr -> next;
		}
		
		command *new = (command*)malloc(sizeof(command));
		if (!new) {
			free(new);
			fprintf(stderr, "Out of memory\n");
			return 1;
		}
		new -> name = strdup(cmd);
		new -> next = NULL;
		if (curr -> cmd) {
			command *currCmd = curr -> cmd;
			while (currCmd -> next) {
				currCmd = currCmd -> next;
			}
			currCmd -> next = new;
			currCmd -> next = new;
		}
		else {
			curr -> cmd = new;
		}
		
	}
	else {
		fprintf(stderr, "Invalid command\n");
		return 1;
	}
	
	return 0;
	
}

/* createTarget(parent) -- creates a new target with the name parent
*/
target* createTarget(char *parent) {
	target *new = (target*)malloc(sizeof(target));
	new -> name = strdup(parent);
	new -> cmd = NULL;
	new -> next = NULL;
	new -> dependentOn = NULL;
	new -> visited = 0;
	new -> seen = 0;
	
	return new;
}

/*
* addDependency(str, parent) -- Adds the dependency with the name str to the parent target
*/
void addDependency(char str[], target *parent) {
	dependency *new = (dependency*)malloc(sizeof(dependency));
	new -> name = strdup(str);
	new -> next = NULL;
	new -> seen = 0;
	
	if (!parent -> dependentOn) {
		parent -> dependentOn = new;
	}
	else {
		dependency *curr = parent -> dependentOn;
		while (curr -> next) {
			curr = curr -> next;
		}
		curr -> next = new;
	}
}

/*
* targetExists(myTarget) -- Determines if myTarget exists in the dependency graph
*/
int targetExists(target *myTarget) {
	target *curr = head;
	while (curr) {
		if (strcmp(curr -> name, myTarget -> name) == 0) {
			return 1;
		}
		curr = curr -> next;
	}
	return 0;
}

/*
* postOrder(myTarget) -- Prints the post order traversal of the dependencies of myTarget
*/
void postOrder(target *myTarget) {
	
	if (myTarget -> visited == 1) {
		return;
	}
	myTarget -> visited = 1;
	
	dependency *curr = myTarget -> dependentOn;
	while (curr) {
		target *toTarget = getTarget(curr -> name);
		if (toTarget) {
			postOrder(toTarget);
		}
		else {
			if (curr -> seen == 0) {
				setAllSeen(curr -> name);
				printf("%s\n", curr -> name);
			}
		}
		
		curr = curr -> next;
		
	}
	
	if (myTarget -> seen == 0) {
		setAllSeen(myTarget -> name);
		printf("%s\n", myTarget -> name);
	}
	command *currCmd = myTarget -> cmd;
	while (currCmd) {
		printf("%s\n", currCmd -> name);
		currCmd = currCmd -> next;
	}

	
	
	/*
	
	target *curr = head;
	while (curr) {
		printf("%s\n", curr -> name);
		dependency *currd = curr -> dependentOn;
		while (currd) {
			printf("%s\n", currd -> name);
			currd = currd -> next;
		}
			
			
		command *Cmd = curr -> cmd;
		while (Cmd) {
			printf("%s\n", Cmd -> name);
			Cmd = Cmd -> next;
		}
		curr = curr -> next;
	}
	*/
}

/*
* getTarget(name) -- Gets the target with the same name as name
*/
target* getTarget(char *name) {
	target *curr = head;
	while (curr) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	return NULL;
}

/*
* stripWhitespace(str) -- Strips all of the whitespace from str
*/
void stripWhitespace(char *str) {
	while (isspace(*str)) {
		str++;
	}
	
	char *end = str + strlen(str) - 1;
	while (end > str && isspace(*end)) {
		end--;
	}
	
	*(end+1) = '\0';
}

/*
* setAllSeen(name) -- Sets all targets and dependencies with the same name as name to seen
*/
void setAllSeen(char *name) {
	target *curr = head;
	while (curr) {
		if (strcmp(curr -> name, name) == 0) {
			curr -> seen = 1;
		}
		dependency *currdp = curr -> dependentOn;
		while (currdp) {
			if (strcmp(currdp -> name, name) == 0) {
				currdp -> seen = 1;
			}
			currdp = currdp -> next;
		}
		curr = curr -> next;
	}
}

/*
* freeAll() -- Frees the dependency graph
*/
void freeAll() {
	
	target *target1 = NULL;
	target *target2 = NULL;
	target1 = head;
	while (target1) {
		dependency *dp1;
		dependency *dp2;
		target2 = target1 -> next;
		
		dp1 = target1 -> dependentOn;
		while (dp1) {
			
			dp2 = dp1 -> next;
			free(dp1 -> name);
			free(dp1);
			dp1 = dp2;
		}
		
		command *cmd1;
		command *cmd2;
		cmd1 = target1 -> cmd;
		while (cmd1) {
			
			cmd2 = cmd1 -> next;
			free(cmd1 -> name);
			free(cmd1);
			
			cmd1 = cmd2;
			
		}
		
		free(target1 -> name);
		free(target1);
		target1 = target2;
	}
	
}