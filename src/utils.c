void log_error(Error error) {
	char *error_message;
	switch (error) {
		case ERROR_BAD_ALLOCATION:
			error_message = "Failed to allocate memory.";
			break;
		case ERROR_NULL_POINTER:
			error_message = "Unexpected NULL pointer.";
			break;
		default:
			error_message = "Generic error.";
			break;
	}
	fprintf(stderr, "ERROR: %s\n", error_message);
}

void log_error_and_exit(Error error) {
	log_error(error);
	exit(EXIT_FAILURE);
}

void log_error_message(char *message) {
	fprintf(stderr, "ERROR: %s\n", message);
}

void log_error_message_and_exit(char *message) {
	log_error_message(message);
	exit(EXIT_FAILURE);
}

void log_debug(char *message) {
	#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s\n", message);
	#endif
}

void log_message(char *message) {
	fprintf(stdout, "LOG: %s\n", message);
}

void log_string(String message) {
	printf("%s\n", message.buffer);
}