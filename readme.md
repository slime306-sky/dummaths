This repo contains a simple step-by-step calculator.
My plan is to make it so that when we give input, it will do the calculation according to what’s needed—like whether the user wants the answer in fractions or not.

The output will be in Markdown format, so it’s easy to use anywhere.
Now, you might be thinking: “Why make this? There are already tools on the internet that do that, right?”
Yeah… but here’s the thing—I’m not great at math, so my goal is to learn it by building this myself.
And hey, I didn’t make those other tools, so this one’s mine!






## Mind-map 
```
User input
   ↓
Lexer → Tokens
   ↓
Parser → AST
   ↓
Problem type detection
   ↓
Step-by-step solver (rules for type)
   ↓
Pretty output formatter (Markdown)
```


