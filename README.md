# 📱 Social Media Simulation in C

## 📝 Project Overview

This project is a C program that simulates a simple social media platform, featuring user and post caching, user interactions like following and liking posts, and doubly linked lists for data management.

---

## 🏛️ Key Features

User & Post Cache: Efficient data retrieval using LRU cache mechanism.

Follow Users: Users can follow others, increasing their followers and following counts.

Like Posts: Users can like posts, incrementing like counters.

Print Followers in Reverse: Display the followers of each cached user in reverse order.

---

## 🛠️ Key Functions

fetchUser(int userId) – Retrieves a user from cache or database.

fetchPost(int postId) – Retrieves a post from cache or database.

followUser(int followerId, int followedId) – Makes a user follow another user.

likePost(int userId, int postId) – Allows a user to like a post.

printFollowersInReverse() – Prints followers of each user in cache in reverse.

---

## ▶️ How to Run

Compile and run the program:

gcc socialmedia.c -o socialmedia
./socialmedia

Ensure socialmedia.h and any necessary database setup is available in the working directory.

---

## 📤 Sample Output

User: 1, Username: Alice, Followers: 5, Following: 3, Liked: 2
Post: 2, Author: Bob, Likes: 10
Followers of all users in UserCache in reverse:
Followers of user with id 1 in reverse:
[5] [3] [2]

---

## 🧑‍💻 Dependencies

C Compiler (gcc)

socialmedia.h (header file defining User, Post, Cache, and Node structures)

---

## 📫 Contact

Feel free to reach out for questions or contributions!
