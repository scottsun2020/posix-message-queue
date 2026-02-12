#include <mqueue.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <fcntl.h>


int main() {
    //1. initialize the message queue attributes
    struct mq_attr attr = {};

    //2. set the maximum number of messages and the maximum message size
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;

    //3. create and open the message queue for reading
    mqd_t mq = mq_open("/my_queue", O_CREAT | O_RDONLY, 0644, &attr);

    //4. check if the message queue was opened successfully
    if (mq == (mqd_t)-1) {
        perror("Error opening message queue");
        return 1;
    }

    std::cout << "Server starts. Waiting for messages..." << std::endl;

    //5. create a buffer 256 + 1 for '\0' to receive the message
    char buffer[257];

    //Infinite Loop to keep receiving messages until "exit" is received
    while (true) {
        //6. call mq_receive to read the message from the queue

        ssize_t bytes = mq_receive(mq, buffer, 256, NULL);

        //7. send the message to the queue
        if (bytes == -1) {
            perror("Error receiving message");
            mq_close(mq);
            return 1;
        }

        //8. null-terminate the received message
        buffer[bytes] = '\0'; 

        std::cout << "Received message: " << buffer << std::endl;

        //9.check if message is "exit"
        if (strcmp(buffer, "exit") == 0) {
            std::cout << "Received exit message. Exiting." << std::endl;
            break;
        }
    }

    //10. close the message queue
    mq_close(mq);   
    //11.unlink the message queue
    mq_unlink("/my_queue");

    std::cout << "Message queue closed and unlinked. Server exiting." << std::endl;

    return 0;
}   
