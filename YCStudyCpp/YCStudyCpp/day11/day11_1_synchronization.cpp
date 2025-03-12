//
// Created by 杨充 on 2024/6/27.
//
#include <iostream>

using namespace std;

//11.1.1.1 为何需要线程安全锁
void test11_1_1_1();
//11.1.1.2 什么是线程安全锁
void test11_1_1_2();
//11.1.1.3 常见的线程同步机制
void test11_1_1_3();

int main() {
    test11_1_1_1();
    test11_1_1_2();
    test11_1_1_3();
    return 0;
}

//11.1.1.1 为何需要线程安全锁
void test11_1_1_1() {
    //在多线程编程中，线程安全锁（Thread Safety Lock）是一种用于保护共享资源的机制。
    //多个线程同时访问和修改共享资源时，如果没有适当的同步机制，可能会导致数据竞争（Data Race）和不确定的行为。
    //
    //以下是一些原因说明为何需要线程安全锁：
    //
    //数据竞争：当多个线程同时读取和写入共享资源时，可能会发生数据竞争。数据竞争会导致未定义的行为，例如读取到无效或不一致的数据。
    //线程安全锁可以防止多个线程同时访问共享资源，从而避免数据竞争。
    //
    //原子性操作：某些操作需要保证原子性，即在多线程环境下不会被中断。例如，对于一个计数器，如果多个线程同时对其进行递增操作，可能会导致计数器值不正确。
    //线程安全锁可以确保这样的操作在同一时间只能由一个线程执行。
    //
    //临界区保护：在多线程环境中，某些代码块或操作可能需要在同一时间只能由一个线程执行，以保证数据的一致性。
    //线程安全锁可以用于创建临界区，确保只有一个线程可以进入临界区执行相关代码。
    //
    //同步和顺序性：线程安全锁还可以用于同步多个线程之间的操作顺序，以确保特定的顺序和同步语义。
    //例如，使用互斥锁和条件变量可以实现线程之间的等待和通知机制，以便线程按照特定的顺序执行。
}

//11.1.1.2 什么是线程安全锁
void test11_1_1_2() {
    //线程安全锁（Thread Safety Lock）是一种用于保护共享资源的机制，用于确保在多线程环境中对共享资源的访问是安全和可靠的。它是多线程编程中常用的同步机制之一。
    //
    //线程安全锁的主要目的是防止多个线程同时访问和修改共享资源，从而避免数据竞争（Data Race）和不确定的行为。它提供了一种互斥（Mutex）机制，即一次只允许一个线程访问被保护的代码块或数据。
    //
    //在C++中，常用的线程安全锁是互斥锁（Mutex）。互斥锁提供了两个基本操作：
    //
    //加锁（Lock）：当一个线程需要访问共享资源时，它会尝试获取互斥锁。如果互斥锁当前没有被其他线程持有，则该线程成功获取锁，并可以进入临界区执行相关代码。如果互斥锁已经被其他线程持有，则该线程会被阻塞，直到锁被释放。
    //
    //解锁（Unlock）：当一个线程完成对共享资源的访问后，它会释放互斥锁，使其他线程有机会获取锁并访问共享资源。
    //
    //通过使用互斥锁，可以确保在同一时间只有一个线程可以访问共享资源，从而避免了数据竞争和不确定的行为。互斥锁提供了一种简单而有效的方式来保护共享资源的访问，使多线程程序能够正确地协调和共享数据。
}

//11.1.1.3 常见的线程同步机制
void test11_1_1_3() {
    //互斥锁（Mutex）：互斥锁是一种最基本的线程同步机制，用于保护共享资源的访问。通过使用互斥锁，只有一个线程可以同时访问被保护的代码块或数据，其他线程必须等待锁的释放。
    //
    //条件变量（Condition Variable）：条件变量用于在多个线程之间进行通信和同步。它允许线程等待某个条件的发生，并在条件满足时被唤醒。条件变量通常与互斥锁一起使用，以确保线程在等待和唤醒时的安全性。
    //
    //信号量（Semaphore）：信号量是一种计数器，用于控制对共享资源的访问。它可以限制同时访问资源的线程数量，并提供了一种机制来同步线程之间的操作。
    //
    //屏障（Barrier）：屏障用于在多个线程之间进行同步，以确保所有线程都达到某个点之前都被阻塞。一旦所有线程都到达屏障点，它们将同时继续执行。
    //
    //原子操作（Atomic Operations）：原子操作是一种线程安全的操作，可以在没有锁的情况下对共享数据进行读取和修改。原子操作保证了操作的原子性，即不会被其他线程中断。
}



