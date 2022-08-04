/*
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2017, Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Simple Apache Kafka producer
 * using the Kafka driver from librdkafka
 * (https://github.com/edenhill/librdkafka)
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

/* Typical include path would be <librdkafka/rdkafka.h>, but this program
 * is builtin from within the librdkafka source tree and thus differs. */
#include "rdkafka.h"


#define COMMON_FILE_PATH_LEN   512
#define BUFFER_MAX_SIZE        2048

static volatile sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop(int sig) {
        run = 0;
        fclose(stdin); /* abort fgets() */
}


/**
 * @brief Message delivery report callback.
 *
 * This callback is called exactly once per message, indicating if
 * the message was succesfully delivered
 * (rkmessage->err == RD_KAFKA_RESP_ERR_NO_ERROR) or permanently
 * failed delivery (rkmessage->err != RD_KAFKA_RESP_ERR_NO_ERROR).
 *
 * The callback is triggered from rd_kafka_poll() and executes on
 * the application's thread.
 */
static void
dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque) {
        if (rkmessage->err)
                fprintf(stderr, "%% Message delivery failed: %s\n",
                        rd_kafka_err2str(rkmessage->err));
        else
                fprintf(stderr,
                        "%% Message delivered (%zd bytes, "
                        "partition %" PRId32 ")\n",
                        rkmessage->len, rkmessage->partition);

        /* The rkmessage is destroyed automatically by librdkafka */
}

int is_dir_exist(const char *dir_path)
{
    DIR *dir;
    if(dir_path==NULL){
        return -2;
    }
    if((dir=opendir(dir_path))==NULL){
        return -1;
    }
    closedir(dir);

    return 0;
}


static int 
is_file_exist(const char* file_path)
{
    if(file_path==NULL){
        return 0;
    }

    if(access(file_path,F_OK)==0){
        return 1;
    }
    return 0;
}

static int 
get_file_suffix(char * file_name, char *suffix)
{
    if(file_name==NULL || suffix==NULL){
        return -1;
    }

    char *p=NULL;
    p=strrchr(file_name,'.');
    if(NULL==p){
        return -1;
    }

    strcpy(suffix,p+1);
    return 0;
}


static int 
rd_kafka_send_msg(char *buf, int len, const char *topic, rd_kafka_t *rk)
{
    char errstr[512];      /* librdkafka API error reporting buffer */
	rd_kafka_resp_err_t err;
    /*
     * Send/Produce message.
     * This is an asynchronous call, on success it will only
     * enqueue the message on the internal producer queue.
     * The actual delivery attempts to the broker are handled
     * by background threads.
     * The previously registered delivery report callback
     * (dr_msg_cb) is used to signal back to the application
     * when the message has been delivered (or failed).
     */
retry:
    err = rd_kafka_producev(
        /* Producer handle */
        rk,
        /* Topic name */
        RD_KAFKA_V_TOPIC(topic),
        /* Make a copy of the payload. */
        RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
        /* Message value and length */
        RD_KAFKA_V_VALUE(buf, len),
        /* Per-Message opaque, provided in
         * delivery report callback as
         * msg_opaque. */
        RD_KAFKA_V_OPAQUE(NULL),
        /* End sentinel */
        RD_KAFKA_V_END);

    if (err) {
            /*
             * Failed to *enqueue* message for producing.
             */
            fprintf(stderr,
                    "%% Failed to produce to topic %s: %s\n", topic,
                    rd_kafka_err2str(err));

            if (err == RD_KAFKA_RESP_ERR__QUEUE_FULL) {
                    /* If the internal queue is full, wait for
                     * messages to be delivered and then retry.
                     * The internal queue represents both
                     * messages to be sent and messages that have
                     * been sent or failed, awaiting their
                     * delivery report callback to be called.
                     *
                     * The internal queue is limited by the
                     * configuration property
                     * queue.buffering.max.messages */
                    rd_kafka_poll(rk,
                                  1000 /*block for max 1000ms*/);
                    goto retry;
            }
    } else {
            fprintf(stderr,
                    "%% Enqueued message (%d bytes) "
                    "for topic %s\n",
                    len, topic);
    }


    /* A producer application should continually serve
     * the delivery report queue by calling rd_kafka_poll()
     * at frequent intervals.
     * Either put the poll call in your main loop, or in a
     * dedicated thread, or call it after every
     * rd_kafka_produce() call.
     * Just make sure that rd_kafka_poll() is still called
     * during periods where you are not producing any messages
     * to make sure previously produced messages have their
     * delivery report callback served (and any other callbacks
     * you register). */
    rd_kafka_poll(rk, 0 /*non-blocking*/);

    return 0;
}


static int 
get_file_line(const char *file_path, const char *topic, rd_kafka_t *rk)
{
    if(!file_path){
        printf("filepath is NULL\n");
        return -1;
    }
    
    FILE       *fp = NULL;
    fp = fopen(file_path, "r");
    if(!fp){
        printf("fopen %s failed !\n",file_path);
        return -1;
    }

    char buff[BUFFER_MAX_SIZE]={0};
    int line=0;
    while(fgets(buff, sizeof(buff), fp)!=NULL){
        line++;
        printf("[line_%04d]%s",line,buff);
		buff[strlen(buff)]='\0';
		rd_kafka_send_msg(buff, strlen(buff)-1, topic, rk);
    }

    if(fp){
        fclose(fp);
        fp = NULL;
    }
    
    char tmp_file[COMMON_FILE_PATH_LEN]={0};
    snprintf(tmp_file, COMMON_FILE_PATH_LEN, "%s.finish", file_path);
    rename(file_path, tmp_file);

    return 0;
}

static int 
traveral_file_and_readline(const char *base_path, const char *topic, rd_kafka_t *rk)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
    char file_path[COMMON_FILE_PATH_LEN];
    char suffix[32];

    struct stat  buf;
    int          result;
    
    if ((dir=opendir(base_path)) == NULL){
        perror("Open dir error...");
        return -1;
    }
    
    while ((ptr=readdir(dir)) != NULL)
    {       
        memset(suffix,0,sizeof(suffix));
        memset(file_path,0,sizeof(file_path));
    
        if(strncmp(ptr->d_name,".",1)==0 || strncmp(ptr->d_name,"..",2)==0){    //current dir OR parrent dir
            continue;
        }
        memset(base,'\0',sizeof(base));
        strcpy(base,base_path);
        strcat(base,"/");
        strcat(base,ptr->d_name);

        if(-1==is_dir_exist(base)) {   //8-file ，10-filelink
            
            if(get_file_suffix(ptr->d_name,suffix)<0){continue;}
                
            if(strcmp(suffix,"json")!=0){continue;}

            
            snprintf(file_path,COMMON_FILE_PATH_LEN ,"%s/%s",base_path,ptr->d_name);
            printf("find json file :%s\n",file_path);
            get_file_line(file_path, topic, rk);
        }else if(0==is_dir_exist(base)){    //dir
            traveral_file_and_readline(base, topic, rk);
        }
        
    }
    
    closedir(dir);
    return 1;
}

int main(int argc, char **argv) {
    rd_kafka_t *rk;        /* Producer instance handle */
    rd_kafka_conf_t *conf; /* Temporary configuration object */
    char errstr[512];      /* librdkafka API error reporting buffer */
    char buf[512];         /* Message value temporary buffer */
    const char *brokers;   /* Argument: broker list */
    const char *topic;     /* Argument: topic to produce to */
    const char *file_dir;

    /*
     * Argument validation
     */
    if (argc != 4) {
            fprintf(stderr, "%% Usage: %s <broker> <topic> <file_dir>\n", argv[0]);
            return 1;
    }

    brokers = argv[1];


    /*kafka topic概念：可以类比为数据库中的一张表或文件系统中的一个文件夹；

      生产者向topic推送消息，消费者从topic中拉取消息
    */
    topic   = argv[2];  
    
    file_dir= argv[3];
    
    
    /*
     * Create Kafka client configuration place-holder
     */
    conf = rd_kafka_conf_new();

    /* Set bootstrap broker(s) as a comma-separated list of
     * host or host:port (default port 9092).
     * librdkafka will use the bootstrap brokers to acquire the full
     * set of brokers from the cluster. */
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr,
                          sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%s\n", errstr);
            return 1;
    }

    /* Set the delivery report callback.
     * This callback will be called once per message to inform
     * the application if delivery succeeded or failed.
     * See dr_msg_cb() above.
     * The callback is only triggered from rd_kafka_poll() and
     * rd_kafka_flush(). */
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    /*
     * Create producer instance.
     *
     * NOTE: rd_kafka_new() takes ownership of the conf object
     *       and the application must not reference it again after
     *       this call.
     */
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk) {
            fprintf(stderr, "%% Failed to create new producer: %s\n",
                    errstr);
            return 1;
    }

    /* Signal handler for clean shutdown */
    signal(SIGINT, stop);

    fprintf(stderr,
            "%% Type some text and hit enter to produce message\n"
            "%% Or just hit enter to only serve delivery reports\n"
            "%% Press Ctrl-C or Ctrl-D to exit\n");




    traveral_file_and_readline((const char *)file_dir, topic, rk);
    
    
    /* Wait for final messages to be delivered or fail.
     * rd_kafka_flush() is an abstraction over rd_kafka_poll() which
     * waits for all messages to be delivered. */
    fprintf(stderr, "%% Flushing final messages..\n");
    rd_kafka_flush(rk, 10 * 1000 /* wait for max 10 seconds */);

    /* If the output queue is still not empty there is an issue
     * with producing messages to the clusters. */
    if (rd_kafka_outq_len(rk) > 0)
            fprintf(stderr, "%% %d message(s) were not delivered\n",
                    rd_kafka_outq_len(rk));

    /* Destroy the producer instance */
    rd_kafka_destroy(rk);

    return 0;
}
