I made this to improve my software skills.

The sub-project "cpu" is a vm. I wanted to make a vm so I could use genetic evolution on it to automagically generate programs that solve problems.
My goal is to make the vm as flexible and extensible as possible. I want people to be able to add their own opcodes really easily.
The reason I want it to be flexible is so I can modify the vm as needed so it can interact with the world outside my computer, maybe with the camera or through a usb robot arm idk. It would be cool if I could add opcodes at runtime so maybe ill do that.

The sub-project "Evolutionary Assembly" is the genetic programming stuff and it actually has a working example.
The algorithm is inspired from NEAT, the difference obviously being that I didn't use neural networks.

# Building
IT uses cmake 3.0.0 and the dependencies are cxxtest and Threads.

