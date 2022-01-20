#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <api/prelude.h>
#include <helpers/system.h>
#include <log/prelude.h>

#if defined(_WIN32) || defined(_WIN64)
  #define HELLO_WORLD "examples\\hello\\hello.day"
  #define HELLO_WORLD_EXECUTABLE "out\\test_hello_world.exe"
  #define HELLO_OUTPUT_FILE "out\\output.txt"
#else
  #define HELLO_WORLD "examples/hello/hello.day"
  #define HELLO_WORLD_EXECUTABLE "out/test_hello_world"
  #define HELLO_OUTPUT_FILE "out/output.txt"
#endif

#define HELLO_WORLD_OUTPUT "Hello world"

#define BUFFER_SIZE 1024

bool test_hello_world();

bool test_hello_world() {
  LOG_INFO("Running test [test_hello_world] %s %s", HELLO_WORLD, HELLO_WORLD_EXECUTABLE);

  int error = daybreak_compile(HELLO_WORLD, HELLO_WORLD_EXECUTABLE);
  if (error != 0) {
    LOG_ERROR("Failed compiling hello world program via compiler API");
    return false;
  }

  FILE* output_file = fopen(HELLO_OUTPUT_FILE, "w+");
  error = run_command(HELLO_WORLD_EXECUTABLE, output_file);
  fclose(output_file);

  output_file = fopen(HELLO_OUTPUT_FILE, "r");
  
  char buffer[BUFFER_SIZE];
  memset(buffer, '\0', sizeof(char) * BUFFER_SIZE);
  while (fgets(buffer, BUFFER_SIZE, output_file)) {
    if (strcmp(buffer, HELLO_WORLD_OUTPUT "\n") != 0) {
      fclose(output_file);
      LOG_ERROR("Failed to output '%s', got '%s'", HELLO_WORLD_OUTPUT, buffer);
      return false;
    }
  }
  fclose(output_file);

  return true;
}