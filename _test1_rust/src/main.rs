use std::thread;
use std::time::Duration;

const NUM_THREADS: usize = 5;

fn main() {
    let thread_handles: Vec<_> = (0..NUM_THREADS).map(|i| {
        println!("IN MAIN: Creating thread {i}.");
        thread::spawn(move || {
            let sleep_time = Duration::from_secs(rand::random::<u64>() % NUM_THREADS as u64);
            println!("THREAD {i}: Started.");
            println!("THREAD {i}: Will be sleeping for {} seconds.", sleep_time.as_secs());
            thread::sleep(sleep_time);
            println!("THREAD {i}: Ended.")
        })
    }).collect();
    
    println!("IN MAIN: All threads are created.");
    
    for (i, thread) in thread_handles.into_iter().enumerate() {
        thread.join().unwrap();
        println!("IN MAIN: Thread {i} has ended.")
    }
    
    println!("MAIN program has ended.");
}
