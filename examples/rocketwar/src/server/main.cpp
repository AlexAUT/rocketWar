#include "server.hpp"

const int PORT = 14441;

int main()
{
  Server server{PORT};

  server.run();

  return 0;
}
