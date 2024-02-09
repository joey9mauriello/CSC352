#include "utils.h"

/*
* File: utils.c
* Author: Joey Mauriello
* Purpose: This program defines all functions used for mymake.c
*/

node *head = NULL;
int cmdRan = 0;

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
			else if (*line == '\t') {
				err = addCmd(line);
				if (err == 1) {
					free(line);
					return 1;
				}
			}
			else {
				fprintf(stderr, "Invalid line\n");
				return 1;
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
	
	node *myTarget = getTargetNew(fp);
	if (!myTarget) {
		fprintf(stderr, "Invalid target\n");
		return 1;
	}
	
	int err = postOrder(myTarget);
	if (err != 0) {
		return 1;
	}
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
	node *new = getTarget(parent);
	if (new == NULL) {
		new = createTarget(parent);
		if (!new) {
			free(new -> name);
			new -> name = NULL;
			free(new);
			
			new = NULL;
			return 1;
		}
		if (!head) {
			head = new;
		}
		else {
			node *curr = head;
			while (curr -> next) {
				curr = curr -> next;
			}
			curr -> next = new;
		}
	}
	new -> target = 1;
	
	
	
	children[strlen(children)-1] = '\0';
	while (sscanf(ptr, "%64s%n", str, &n) > 0) {
		stripWhitespace(str);
		addDependency(str, new);
		ptr += n;
	}
	
	
	
	return 0;

	
}

/*
* addCmd(cmd) -- Adds cmd to the newest target in the dependency graph
*/
int addCmd(char *cmd) {
	
	if (cmd[0] == '\t') {
		cmd++;
		if (cmd[strlen(cmd)-1] == '\n') {
			cmd[strlen(cmd)-1] = '\0';
		}
		if (!head) {
			fprintf(stderr, "No target for command\n");
			return 1;
		}
		node *curr = getLastTarget();
		
		command *new = (command*)malloc(sizeof(command));
		if (!new) {
			
			free(new);
			new = NULL;
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
node* createTarget(char *parent) {
	node *new = (node*)malloc(sizeof(node));
	if (!new) {
		free(new);
		new = NULL;
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}
	new -> name = strdup(parent);
	new -> dependency = NULL;
	new -> cmd = NULL;
	new -> next = NULL;
	new -> visited = 0;
	new -> target = 0;
	new -> completed = 0;
	new -> mustBuild = 0;
	
	return new;
}

/*
* addDependency(str, parent) -- Adds the dependency with the name str to the parent target
*/
int addDependency(char str[], node *parent) {
	node *new = createTarget(str);
	edge *newe = (edge*)malloc(sizeof(edge));
	if (!newe) {
		free(new -> name);
		new -> name = NULL;
		free(new);
		
		new = NULL;
		
		
		fprintf(stderr, "Out of memory\n");
		return 1;
	}
	newe -> to = NULL;
	newe -> next = NULL;
	if (targetExists(new) == 1) {
		free(new -> name);
		new -> name = NULL;
		free(new);
		
		new = NULL;
		
		newe -> to = getTarget(str);
		if (!newe -> to) {
			return 1;
		}
	}
	else {
		newe -> to = new;
		
		if (!head) {
			head = new;
		}
		else {
			node *curr = head;
			while (curr -> next) {
				curr = curr -> next;
			}
			curr -> next = new;
		}
	}
	
	if (!parent -> dependency) {
		parent -> dependency = newe;
	}
	else {
		edge *curr = parent -> dependency;
		while (curr -> next) {
			curr = curr -> next;
		}
		curr -> next = newe;
	}
	return 0;
}

/*
* targetExists(myTarget) -- Determines if myTarget exists in the dependency graph
*/
int targetExists(node *myTarget) {
	node *curr = head;
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
int postOrder(node *myTarget) {
	struct stat s;
	int retval;
	
	if (myTarget -> visited == 1) {
		return 0;
	}
	myTarget -> visited = 1;
	
	retval = stat(myTarget -> name, &s);
	if (retval != 0) {
		if (myTarget -> target == 0) {
			fprintf(stderr, "File does not exist\n");
			return 1;
		}
		else {
			myTarget -> mustBuild = 1;
		}
	}
	edge *curr = myTarget -> dependency;
	while (curr) {
		postOrder(curr -> to);
		if (curr -> to -> completed == 0) {
			fprintf(stderr, "Cycle found\n");
		}
		else if (myTarget -> mustBuild == 0) {
			struct stat s1;
			retval = stat(curr -> to -> name, &s1);
			if (retval != 0 || (s1.st_mtim.tv_sec > s.st_mtim.tv_sec)) {
				myTarget -> mustBuild = 1;
			}
			
		}
		curr = curr -> next;
	}
	if (myTarget -> mustBuild == 1) {
		command *curr = myTarget -> cmd;
		while (curr) {
			printf("%s\n", curr -> name);
			int retval = system(curr -> name);
			if (retval == 0) {
				cmdRan = 1;
			}
			else {
				fprintf(stderr, "Command failed\n");
				return 1;
			}
			stat(myTarget -> name, &s);
			curr = curr -> next;
		}
	}
	myTarget -> completed = 1;
	return 0;

}

/*
* getTarget(name) -- Gets the target with the same name as name
*/
node* getTarget(char *name) {
	node *curr = head;
	while (curr) {
		if (strcmp(curr -> name, name) == 0) {
			return curr;
		}
		curr = curr -> next;
	}
	return NULL;
}

node* getTargetNew(char *name) {
	node *curr = head;
	while (curr) {
		if (strcmp(curr -> name, name) == 0 && curr -> target == 1) {
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
* freeAll() -- Frees the dependency graph
*/
void freeAll() {
	
	node *target1 = NULL;
	node *target2 = NULL;
	target1 = head;
	while (target1) {
		edge *dp1;
		edge *dp2;
		target2 = target1 -> next;
		
		dp1 = target1 -> dependency;
		while (dp1) {
			
			dp2 = dp1 -> next;
			free(dp1);
			dp1 = dp2;
		}
		
		command *cmd1;
		command *cmd2;
		cmd1 = target1 -> cmd;
		while (cmd1) {
			cmd2 = cmd1;
			cmd1 = cmd1 -> next;
			free(cmd2 -> name);
			free(cmd2);
			
			
		}
		
		free(target1 -> name);
		free(target1);
		target1 = target2;
	}
	
}

/*
* printGraph() -- prints the dependency graph
*/
void printGraph() {
	node *currn = head;
	while (currn) {
		printf("%s,%d: ", currn -> name, currn -> target);
		edge *curre = currn -> dependency;
		while (curre) {
			printf("%s,%d ", curre -> to -> name, curre -> to -> target);
			curre = curre -> next;
		}
		printf("\n");
		
		command *currc = currn -> cmd;
		while (currc) {
			printf("%s\n", currc -> name);
			currc = currc -> next;
		}
		
		currn = currn -> next;
		
	}
}

/*
* getLastTarget() -- returns the last target in the dependency graph
*/
node* getLastTarget() {
	node *curr = head;
	node *last = NULL;
	while (curr) {
		if (curr -> target == 1) {
			last = curr;
		}
		curr = curr -> next;
	}
	return last;
}
