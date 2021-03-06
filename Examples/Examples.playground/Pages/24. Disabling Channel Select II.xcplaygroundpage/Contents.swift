import Venice
import Darwin

let channelA = Channel<String>()
let channelB = Channel<String>()

co(channelA <- "a")
co(channelB <- "b")

select { when in
    if arc4random_uniform(2) == 0 {
        print("disabled channel b")
        when.receiveFrom(channelA) { value in
            print("received \(value) from channel a")
        }
    } else {
        print("disabled channel a")
        when.receiveFrom(channelB) { value in
            print("received \(value) from channel b")
        }
    }
}
