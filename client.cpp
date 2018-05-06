
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace zmqpp;

int main(int argc, char *argv[]) {
  const string endpoint = "tcp://localhost:4242";

  // initialize the 0MQ context
  context context;

  // generate a push socket
  socket_type type = socket_type::req;
  socket socket (context, type);

  // open the connection
  cout << "Opening connection to " << endpoint << "..." << endl;
  socket.connect(endpoint);

  // send a message
  cout << "Sending params" << endl;
  message params;
  // compose a message from a K and a has_name

  params << 3 << 1;
  socket.send(params);
  
  message answer;
  socket.receive(answer);

  int error, time;
  answer >> error >> time;
  cout << "Error : " << error << " Time : " << time <<endl;
  cout << "Sent message." << endl;
  cout << "Finished." << endl;
}