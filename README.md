## __MAGCF — Multi-Agent Generative Character Framework__

MAGCF is an experimental AI-powered NPC simulation built in Unreal Engine 5.

This started as a personal challenge to see how far I could push AI-driven NPC behavior inside a game environment. It's also my first AI project, so a lot of it has been learning by building, making mistakes, and figuring things out along the way.

The project uses a local Ollama3.2 model running on a Ryzen 5 3600, GTX 1650, and 16GB of RAM. There is no backend server or external AI infrastructure; everything runs directly through Unreal Engine. My focus was to keep the project simple and understand how LLMs could be used to drive NPC decision-making in a real-time simulation.

For the visual, character models and animations come from [Mixamo](https://www.mixamo.com/), which allowed me to spend more time experimenting with AI systems instead of creating art assets.

## __What it Does__

Right now, NPCs can perform a small set of actions:

Eat
Dance
Sit
Exercise
Talk over the phone

The action pool is intentionally limited because the goal wasn't to build a complete game, but to explore the challenges of AI-driven behavior in a simulated environment.

## __Challenges__

The biggest challenge has been architecture.

As more NPC states and stats are introduced, keeping track of everything becomes surprisingly difficult. Managing state decay, decision-making, and action selection across multiple NPCs quickly turns into a design problem rather than just a programming problem.

At the moment, NPCs sometimes get stuck repeating the same actions, and some behaviors don't work as consistently as I'd like. I think a lot of these issues could be improved with better state tracking, personality systems, and overall AI architecture.

Since I'm working on this solo and came into it with no game development experience, the project is far from perfect—but it's been a great way to learn both Unreal Engine and AI system design.

## __Why I Built It__

While researching AI NPCs, I noticed that most projects focus on conversations, basically giving players the ability to chat with NPCs or ask them to do something.

What interests me more is the idea of NPCs acting on their own inside a living world. Even in this early prototype, the goal is to explore what happens when characters make decisions based on their own needs and states instead of waiting for player input.

## __Future Ideas__

One thing I find particularly exciting is AI-generated quests.

Instead of developers handcrafting every quest, NPCs could eventually generate requests, objectives, and storylines naturally through conversations with players. If done well, that could create much more dynamic and personal gameplay experiences.

If I continue developing MAGCF or rebuild it from scratch. I would probably move the AI systems into a dedicated backend service. Building everything directly inside Unreal Engine was great for experimentation, but separating the simulation and AI layers would make the project much easier to scale and maintain.

## __Current Status__

MAGCF is very much an experimental prototype.

There are plenty of rough edges, unfinished systems, and design decisions that I would approach differently today. Still, for a first AI project, it's been a rewarding experience and has given me a much deeper appreciation for how difficult it is to build believable autonomous NPCs

## __Feedback Welcome__

I'm always interested in hearing ideas, suggestions, or seeing how other people approach similar problems. Feel free to reach out or share your work.
