#include "socialmedia.h"
#include <stdlib.h>

Database db;
Cache userCache;
Cache postCache;

void printUser(User *user) {
  printf("User: %d, Username: %s, Followers: %d, Following: %d, Liked: %d\n",
         user->userId, user->username, user->numOfFollowers,
         user->numOfFollowing, user->liked);
}

void printPost(Post *post) {
  printf("Post: %d, Author: %s, Likes: %d\n", post->postId,
         post->author->username, post->likes);
}

void printUserCache() {
  Node* node_ptr;
  printf("UserCache:\n");
  node_ptr=userCache.head;
  while(node_ptr!=NULL){
      printUser(node_ptr->data);
      node_ptr=node_ptr->next;
  }
}

void printPostCache() {
  Node* node_ptr;
  printf("PostCache:\n");
  node_ptr=postCache.tail;
  while(node_ptr!=NULL){
      printPost(node_ptr->data);
      node_ptr=node_ptr->prev;
  }
}

Node *createNode(void *data) {
    Node* new_node;
    new_node=(Node*)malloc(sizeof(Node));
    new_node->data= data;
    new_node->next=NULL;
    new_node->prev=NULL;
    return new_node;
}

void appendToCache(Cache *cache, void *data) {
    Node* new_node;
    new_node=createNode(data);
    if (cache->head==NULL){
        cache->head=new_node;
        cache->tail=new_node;
        cache->size++;
    }
    else if(cache->capacity>cache->size){
        cache->head->prev=new_node;
        new_node->next=cache->head;
        cache->head=new_node;
        cache->size++;
    }
    else if (cache->capacity==cache->size){
        removeLast(cache);
        appendToCache(cache, data);
    }
}

void moveToFront(Cache *cache, Node *node) {
    Node* temp;
    Node* new_node_ptr;
    int in_or_out=0;
    new_node_ptr=cache->head;
    while (new_node_ptr!=NULL){
        if (new_node_ptr==node){
            in_or_out=1;
            break;
        }
        new_node_ptr=new_node_ptr->next;
    }
    if (in_or_out==0){
        return ;
    }
    if (cache->size==0 || cache->size==1 || cache->head==node){
        return ;
    }
    else if(cache->tail==node && cache->size==2){
        cache->tail->prev->next=NULL;
        cache->tail->next=cache->head;
        cache->head->prev=cache->tail;
        cache->head->next=NULL;
        temp=cache->head;
        cache->head=cache->tail;
        cache->tail=temp;
    }
    else if (cache->tail==node){
        cache->tail->prev->next=NULL;
        cache->tail->next=cache->head;
        cache->head->prev=cache->tail;
        temp=cache->tail;
        cache->tail=cache->tail->prev;
        cache->head=temp;
        cache->head->prev=NULL;
    }
    else{
        node->prev->next=node->next;
        node->next->prev=node->prev;
        node->next=cache->head;
        node->prev=NULL;
        cache->head->prev=node;
        cache->head=node;    
    }
}

void removeLast(Cache *cache) {
    if (cache->head==NULL){
        return ;
    }
    else if (cache->size==1){
        free(cache->head);
        cache->head=NULL;
        cache->tail=NULL;
        cache->size--;
    }
    else{
        cache->tail=cache->tail->prev;
        free(cache->tail->next);
        cache->tail->next=NULL;
        cache->size--;
    }
}

User *fetchUser(int userId) {
    User* user_ptr;
    Node* new_node_ptr;
    int inside=0;
    new_node_ptr=userCache.head;
    if (userCache.head==NULL){
        user_ptr=&(db.users[userId]);
        appendToCache(&userCache, user_ptr);
        return user_ptr;
    }
    while (new_node_ptr!=NULL){
        user_ptr=(User*)new_node_ptr->data;
        if(user_ptr->userId==userId){
            moveToFront(&userCache, new_node_ptr);
            inside=1;
            return user_ptr;
        }
        new_node_ptr=new_node_ptr->next;
    }
    if (inside==0){
        user_ptr=&(db.users[userId]);
        appendToCache(&userCache, user_ptr);
        return user_ptr;
    }
}

Post *fetchPost(int postId) {
    Post* post_ptr;
    Node* new_node_ptr;
    int inside=0;
    new_node_ptr=postCache.head;
    if (postCache.head==NULL){
        post_ptr=&(db.posts[postId]);
        appendToCache(&postCache, post_ptr);
        return post_ptr;
    }
    while (new_node_ptr!=NULL){
        post_ptr=(Post*)new_node_ptr->data;
        if(post_ptr->postId==postId){
            moveToFront(&postCache, new_node_ptr);
            inside=1;
            return post_ptr;
        }
        new_node_ptr=new_node_ptr->next;
    }
    if (inside==0){
        post_ptr=&(db.posts[postId]);
        appendToCache(&postCache, post_ptr);
        return post_ptr;
    }
}

void followUser(int followerId, int followedId) {
  User* follower;
  User* followed;
  Node* follower_node;
  follower=fetchUser(followerId);
  followed=fetchUser(followedId);
  follower->numOfFollowing++;
  followed->numOfFollowers++;
  follower_node=createNode(follower);
  if (followed->followers_head==NULL){
      followed->followers_head=follower_node;
      followed->followers_tail=follower_node;
  }
  else{
      followed->followers_tail->next=follower_node;
      follower_node->prev=followed->followers_tail;
      followed->followers_tail=follower_node;
  }
}

void likePost(int userId, int postId) {
    User* likes;
    Post* liked;
    likes=fetchUser(userId);
    liked=fetchPost(postId);
    likes->liked++;
    liked->likes++;
}

void printFollowersInReverse() {
  Node* current1;
  Node* current2;
  if (userCache.size==0){
      return ;
  }
  printf("Followers of all users in UserCache in reverse:\n");
  current1=userCache.tail;
  while(current1!=NULL){
    current2=((User*)current1->data)->followers_tail;
    printf("Followers of user with id %d in reverse:\n", ((User*)current1->data)->userId);
    while(current2!=NULL){
        printf("[%d] ", ((User*)current2->data)->userId);
        current2=current2->prev;
    }
  printf("\n");
  current1=current1->prev;
  }
}

void freeLinkedList(Node *head) {
  Node *node_ptr, *temp;
  node_ptr=head;
  while (node_ptr!=NULL){
      temp=node_ptr;
      node_ptr=node_ptr->next;
      free(temp);
  }
}