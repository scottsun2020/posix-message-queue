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

    //3. create and open the message queue for writing
    mqd_t mq = mq_open("/my_queue", O_CREAT | O_WRONLY, 0644, &attr);

    //4. check if the message queue was opened successfully
    if (mq == (mqd_t)-1) {
        perror("Error opening message queue");
        return 1;
    }
    
    std::string message;

    while(true) {
        //5. input the message to be sent
        std::cout << "Enter a message to send (type 'exit' to quit): ";
        std::getline(std::cin, message); 

        //6. check sizefor errors
        if (message.size() > attr.mq_msgsize) {
            std::cerr << "Error: Message size exceeds maximum allowed size." << std::endl;
            mq_close(mq);
            continue;;
        }

        //7. send the message to the queue
        if (mq_send(mq, message.c_str(), message.size(), 0) == -1) {
            perror("Error sending message");
            mq_close(mq);
            break;
        }

        std::cout << "Message sent successfully " << message.c_str() << std::endl;

        //8. check if message is "exit"
        if (message == "exit") {
            std::cout << "Exit message sent. Shuting.." << std::endl;
            break;  
        }
    }
    //7. close the message queue
    mq_close(mq);   

    return 0;
}   
