1. 架构图
* ![from http://gityuan.com/2015/12/26/handler-message-framework/](handle.jpg)
* Message：消息分为硬件产生的消息(如按钮、触摸)和软件生成的消息；

* MessageQueue：消息队列的主要功能向消息池投递消息(MessageQueue.enqueueMessage)和取走　消息池的消息(MessageQueue.next)；

* Handler：消息辅助类，主要功能向消息池发送各种消息事件(Handler.sendMessage)和处理相应消息事件(Handler.handleMessage)；

* Looper：不断循环执行(Looper.loop)，按分发机制将消息分发给目标处理者。


２. Message
```
 public final class Message implements Parcelable {
   public int what;
   public int arg1;
   public int arg2;
   Handler target;
   Runnable callback;
   Message next;
   private static Message sPool;
   private static int sPoolSize = 0;
   private static final int MAX_POOL_SIZE = 50;
   ...
   public static Message obtain() {
        synchronized (sPoolSync) {
            if (sPool != null) {
                Message m = sPool;
                sPool = m.next;
                m.next = null;
                m.flags = 0; // clear in-use flag
                sPoolSize--;
                /// M: Add message protect mechanism
                m.hasRecycle = false;
                return m;
            }
        }
        return new Message();
    }

   public static Message obtain(Handler h) {
        Message m = obtain();
        m.target = h;
        return m;
    }
    ...
 }
 　　void recycleUnchecked() {
        // Mark the message as in use while it remains in the recycled object pool.
        // Clear out all other details.
        flags = FLAG_IN_USE;
        what = 0;
        arg1 = 0;
        arg2 = 0;
        obj = null;
        replyTo = null;
        sendingUid = -1;
        when = 0;
        target = null;
        callback = null;
        data = null;
        hasRecycle = true;
        synchronized (sPoolSync) {
            if (sPoolSize < MAX_POOL_SIZE) {
                next = sPool;
                sPool = this;
                sPoolSize++;
            }
        }
    }
```

 * Message obtain() 方法新建一个Message对象或是使用回收的Message对象
 * Message recycleUnchecked() 回收一个不在使用的Message对象．
---

3. Looper

```
public final class Looper {
    private static final String TAG = "Looper";
    // sThreadLocal.get() will return null unless you've called prepare().
    static final ThreadLocal<Looper> sThreadLocal = new ThreadLocal<Looper>();
    private static Looper sMainLooper;  // guarded by Looper.class
    final MessageQueue mQueue;　　//消息队列
    ...
    //构造方法，新建消息队列，并活动当前线程
    private Looper(boolean quitAllowed) {
        mQueue = new MessageQueue(quitAllowed);
        mThread = Thread.currentThread();
    }

    private static void prepare(boolean quitAllowed) {
        if (sThreadLocal.get() != null) {
            throw new RuntimeException("Only one Looper may be created per thread");
        }
        sThreadLocal.set(new Looper(quitAllowed));
    }
    ...
    //获取线程中的looper　对象
    public static Looper myLooper() {
            return sThreadLocal.get();
        }

public static void loop() {
       final Looper me = myLooper();
       if (me == null) {
           throw new RuntimeException("No Looper; Looper.prepare() wasn't called on this thread.");
       }
       final MessageQueue queue = me.mQueue;

       // Make sure the identity of this thread is that of the local process,
       // and keep track of what that identity token actually is.
       Binder.clearCallingIdentity();
       final long ident = Binder.clearCallingIdentity();
      //循环消息队列，队列空退出循环
       for (;;) {
           Message msg = queue.next(); // might block
           if (msg == null) {
               // No message indicates that the message queue is quitting.
               return;
           }

           // This must be in a local variable, in case a UI event sets the logger
           Printer logging = me.mLogging;
           if (logging != null) {
               logging.println(">>>>> Dispatching to " + msg.target + " " +
                       msg.callback + ": " + msg.what);
           }


            //分发消息，调用Ｈanddle 的dispatchMessage(Message msg)
           msg.target.dispatchMessage(msg);

           if (logging != null) {
               logging.println("<<<<< Finished to " + msg.target + " " + msg.callback);
           }

           // Make sure that during the course of dispatching the
           // identity of the thread wasn't corrupted.
           final long newIdent = Binder.clearCallingIdentity();
           if (ident != newIdent) {
               Log.wtf(TAG, "Thread identity changed from 0x"
                       + Long.toHexString(ident) + " to 0x"
                       + Long.toHexString(newIdent) + " while dispatching to "
                       + msg.target.getClass().getName() + " "
                       + msg.callback + " what=" + msg.what);
           }

           msg.recycleUnchecked();
       }
   }

   public void quit() {
        mQueue.quit(false);
    }

  　public void quitSafely() {
          mQueue.quit(true);
      }
    //Looper.quit()方法的实现最终调用的是MessageQueue.quit()方法
```
* MessageQueue.quit()
```
void quit(boolean safe) {
        if (!mQuitAllowed) {
            throw new IllegalStateException("Main thread not allowed to quit.");
        }

        synchronized (this) {
            if (mQuitting) {
                return;
            }
            mQuitting = true;

            if (safe) {
                removeAllFutureMessagesLocked();
            } else {
                removeAllMessagesLocked();
            }

            // We can assume mPtr != 0 because mQuitting was previously false.
            nativeWake(mPtr);
        }
    }
```

4. Handler
* 构造函数
```
public Handler() {
        this(null, false);
    }

public Handler(Callback callback, boolean async) {
            if (FIND_POTENTIAL_LEAKS) {
                final Class<? extends Handler> klass = getClass();
                if ((klass.isAnonymousClass() || klass.isMemberClass() || klass.isLocalClass()) &&
                        (klass.getModifiers() & Modifier.STATIC) == 0) {
                    Log.w(TAG, "The following Handler class should be static or leaks might occur: " +
                        klass.getCanonicalName());
                }
            }

            mLooper = Looper.myLooper();
            if (mLooper == null) {
                throw new RuntimeException(
                    "Can't create handler inside thread that has not called Looper.prepare()");
            }
            mQueue = mLooper.mQueue;
            mCallback = callback;
            mAsynchronous = async;
}
```
* 有参构造函数
```
public Handler(Looper looper) {
    this(looper, null, false);
}

public Handler(Looper looper, Callback callback, boolean async) {
    mLooper = looper;
    mQueue = looper.mQueue;
    mCallback = callback;
    mAsynchronous = async;
}
```
Handler类在构造方法中，可指定Looper，Callback回调方法以及消息的处理方式(同步或异步)，对于无参的handler，默认是当前线程的Looper


5. 消息分发
```
public void dispatchMessage(Message msg) {
        if (msg.callback != null) {
            handleCallback(msg);
        } else {
            if (mCallback != null) {
                if (mCallback.handleMessage(msg)) {
                    return;
                }
            }
            handleMessage(msg);
        }
    }
```
1. 当Message的回调方法不为空时，则回调方法msg.callback.run()，其中callBack数据类型为Runnable,否则进入步骤2；
2. 当Handler的mCallback成员变量不为空时，则回调方法mCallback.handleMessage(msg),否则进入步骤3；
3. 调用Handler自身的回调方法handleMessage()，该方法默认为空，Handler子类通过覆写该方法来完成具体的逻辑。

6. 消息发送
* Handler.sendEmptyMessage()等系列方法最终调用MessageQueue.enqueueMessage(msg, uptimeMillis)，将消息添加到消息队列中，其中uptimeMillis为系统当前的运行时间，不包括休眠时间。
```
private boolean enqueueMessage(MessageQueue queue, Message msg, long uptimeMillis) {
    msg.target = this;
    if (mAsynchronous) {
        msg.setAsynchronous(true);
    }
    return queue.enqueueMessage(msg, uptimeMillis); 【见4.3】
}
```

7. MessageQueue
* 链接native 层
```
private native static long nativeInit();
private native static void nativeDestroy(long ptr);
private native void nativePollOnce(long ptr, int timeoutMillis);
private native static void nativeWake(long ptr);
private native static boolean nativeIsPolling(long ptr);
private native static void nativeSetFileDescriptorEvents(long ptr, int fd, int events);

MessageQueue(boolean quitAllowed) {
       mQuitAllowed = quitAllowed;
       mPtr = nativeInit(); //native 方法，获取native层MessageQueue的对象指针
   }
```
MessageQueue.enqueueMessage(msg, uptimeMillis)对消息进行排序，并判断调用nativeWake

* next()
```
Message next() {
        // Return here if the message loop has already quit and been disposed.
        // This can happen if the application tries to restart a looper after quit
        // which is not supported.
        final long ptr = mPtr;
        if (ptr == 0) {
            return null;
        }

        int pendingIdleHandlerCount = -1; // -1 only during first iteration
        int nextPollTimeoutMillis = 0;
        for (;;) {
            if (nextPollTimeoutMillis != 0) {
                Binder.flushPendingCommands();
            }

            nativePollOnce(ptr, nextPollTimeoutMillis);

            synchronized (this) {
                // Try to retrieve the next message.  Return if found.
                final long now = SystemClock.uptimeMillis();
                Message prevMsg = null;
                Message msg = mMessages;
                if (msg != null && msg.target == null) {
                    // Stalled by a barrier.  Find the next asynchronous message in the queue.
                    do {
                        prevMsg = msg;
                        msg = msg.next;
                    } while (msg != null && !msg.isAsynchronous());
                }
                if (msg != null) {
                    if (now < msg.when) {
                        // Next message is not ready.  Set a timeout to wake up when it is ready.
                        nextPollTimeoutMillis = (int) Math.min(msg.when - now, Integer.MAX_VALUE);
                    } else {
                        // Got a message.
                        mBlocked = false;
                        if (prevMsg != null) {
                            prevMsg.next = msg.next;
                        } else {
                            mMessages = msg.next;
                        }
                        msg.next = null;
                        if (false) Log.v("MessageQueue", "Returning message: " + msg);
                        return msg;
                    }
                } else {
                    // No more messages.
                    nextPollTimeoutMillis = -1;
                }

                // Process the quit message now that all pending messages have been handled.
                if (mQuitting) {
                    dispose();
                    return null;
                }

                // If first time idle, then get the number of idlers to run.
                // Idle handles only run if the queue is empty or if the first message
                // in the queue (possibly a barrier) is due to be handled in the future.
                if (pendingIdleHandlerCount < 0
                        && (mMessages == null || now < mMessages.when)) {
                    pendingIdleHandlerCount = mIdleHandlers.size();
                }
                if (pendingIdleHandlerCount <= 0) {
                    // No idle handlers to run.  Loop and wait some more.
                    mBlocked = true;
                    continue;
                }

                if (mPendingIdleHandlers == null) {
                    mPendingIdleHandlers = new IdleHandler[Math.max(pendingIdleHandlerCount, 4)];
                }
                mPendingIdleHandlers = mIdleHandlers.toArray(mPendingIdleHandlers);
            }

            // Run the idle handlers.
            // We only ever reach this code block during the first iteration.
            for (int i = 0; i < pendingIdleHandlerCount; i++) {
                final IdleHandler idler = mPendingIdleHandlers[i];
                mPendingIdleHandlers[i] = null; // release the reference to the handler

                boolean keep = false;
                try {
                    keep = idler.queueIdle();
                } catch (Throwable t) {
                    Log.wtf("MessageQueue", "IdleHandler threw exception", t);
                }

                if (!keep) {
                    synchronized (this) {
                        mIdleHandlers.remove(idler);
                    }
                }
            }

            // Reset the idle handler count to 0 so we do not run them again.
            pendingIdleHandlerCount = 0;

            // While calling an idle handler, a new message could have been delivered
            // so go back and look again for a pending message without waiting.
            nextPollTimeoutMillis = 0;
        }
    }
```
native方法
```
void NativeMessageQueue::pollOnce(JNIEnv* env, int timeoutMillis) {
    mInCallback = true;
    mLooper->pollOnce(timeoutMillis);
    mInCallback = false;
    if (mExceptionObj) {
        env->Throw(mExceptionObj);
        env->DeleteLocalRef(mExceptionObj);
        mExceptionObj = NULL;
    }
}
```
Looper.pollOnce()
```
int Looper::pollOnce(int timeoutMillis, int* outFd, int* outEvents, void** outData) {
    int result = 0;
    for (;;) {
        while (mResponseIndex < mResponses.size()) {
            const Response& response = mResponses.itemAt(mResponseIndex++);
            int ident = response.request.ident;
            if (ident >= 0) {
                int fd = response.request.fd;
                int events = response.events;
                void* data = response.request.data;
#if DEBUG_POLL_AND_WAKE
                ALOGD("%p ~ pollOnce - returning signalled identifier %d: "
                        "fd=%d, events=0x%x, data=%p",
                        this, ident, fd, events, data);
#endif
                if (outFd != NULL) *outFd = fd;
                if (outEvents != NULL) *outEvents = events;
                if (outData != NULL) *outData = data;
                return ident;
            }
        }

        if (result != 0) {
#if DEBUG_POLL_AND_WAKE
            ALOGD("%p ~ pollOnce - returning result %d", this, result);
#endif
            if (outFd != NULL) *outFd = 0;
            if (outEvents != NULL) *outEvents = 0;
            if (outData != NULL) *outData = NULL;
            return result;
        }

        result = pollInner(timeoutMillis);
    }
}
```
