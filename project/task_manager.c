#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#define MAX_TASKS 100

	// Structure to represent a task
	struct Task {
		char description[100];
		int completed;
	};

	// Function to add a task to the task list
	void addTask(struct Task tasks[], int *taskCount, const char *description) {
		if (*taskCount < MAX_TASKS) {
			struct Task newTask;
			strcpy(newTask.description, description);
			newTask.completed = 0; // Set as not completed by default
			tasks[*taskCount] = newTask;
			(*taskCount)++;
			printf("Task added successfully.\n");
		} else {
			printf("Task list is full. Cannot add more tasks.\n");
		}
	}

	// Function to view all tasks
	void viewTasks(const struct Task tasks[], int taskCount) {
		if (taskCount > 0) {
			printf("Tasks:\n");
			for (int i = 0; i < taskCount; ++i) {
				printf("%d. %s - %s\n", i + 1, tasks[i].description, tasks[i].completed ? "Completed" : "Not Completed");
			}
		} else {
			printf("No tasks available.\n");
		}
	}

	// Function to mark a task as completed
	void completeTask(struct Task tasks[], int taskCount, int taskIndex) {
		if (taskIndex >= 0 && taskIndex < taskCount) {
			tasks[taskIndex].completed = 1;
			printf("Task marked as completed.\n");
		} else {
			printf("Invalid task index.\n");
		}
	}

	// Function to delete a task
	void deleteTask(struct Task tasks[], int *taskCount, int taskIndex) {
		if (taskIndex >= 0 && taskIndex < *taskCount) {
			for (int i = taskIndex; i < *taskCount - 1; ++i) {
				tasks[i] = tasks[i + 1];
			}
			(*taskCount)--;
			printf("Task deleted successfully.\n");
		} else {
			printf("Invalid task index.\n");
		}
	}

	int main() {
		struct Task tasks[MAX_TASKS];
		int taskCount = 0;

		int choice;
		do {
			printf("\nTask Manager Menu:\n");
			printf("1. Add Task\n");
			printf("2. View Tasks\n");
			printf("3. Mark Task as Completed\n");
			printf("4. Delete Task\n");
			printf("0. Exit\n");

			printf("Enter your choice: ");
			scanf("%d", &choice);

			switch (choice) {
				case 1: {
					char description[100];
					printf("Enter task description: ");
					scanf(" %[^\n]s", description);
					addTask(tasks, &taskCount, description);
					break;
				}
				case 2:
					viewTasks(tasks, taskCount);
					break;
				case 3: {
					int taskIndex;
					printf("Enter the index of the task to mark as completed: ");
					scanf("%d", &taskIndex);
					completeTask(tasks, taskCount, taskIndex - 1);
					break;
				}
				case 4: {
					int taskIndex;
					printf("Enter the index of the task to delete: ");
					scanf("%d", &taskIndex);
					deleteTask(tasks, &taskCount, taskIndex - 1);
					break;
				}
				case 0:
					printf("Exiting the Task Manager. Goodbye!\n");
					break;
				default:
					printf("Invalid choice. Please enter a valid option.\n");
			}
		} while (choice != 0);

		return 0;
	}
