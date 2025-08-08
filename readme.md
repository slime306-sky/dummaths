# Step-by-Step Math Solver — Build Plan

## 1. Input Stage (a.k.a. “What the heck did the user type?”)
**Goal:** Take raw text from the user (e.g., `2x + 3 = 7`) and prepare it for your solver.

**Tasks:**
1. Read the input — handle spaces, quit commands, etc.
2. Normalize symbols — e.g., convert `^` → `**`, `×` → `*`, `÷` → `/`.
3. Handle implicit multiplication — e.g., `2x` → `2 * x`, `(x)(y)` → `(x) * (y)`.
4. Detect expression vs. equation — check for the `=` sign.

*(A lexer would break the string into tokens like NUM, VAR, OP, PAREN.)*

---

## 2. Parsing Stage (a.k.a. “Understanding the sentence”)
**Goal:** Convert tokens into a structure your program understands (AST).

**Tasks:**
1. Build a parser — write your own (shunting yard) or use a built-in parser (Python `ast`, JavaScript `acorn`).
2. Output an Abstract Syntax Tree (AST).

Example for `2 * (3 + 4)`:
```
(*)
/   \
2    (+)
     / \
    3   4
```

---

## 3. Problem Type Detection
**Goal:** Decide what kind of problem you’re solving.

**Types:**
- No `=` → Expression (just evaluate).
- One `=` + one variable → Linear or Quadratic equation.
- More complex → reject or use a symbolic math library.

---

## 4. Step-by-Step Evaluation
**Expressions:**
1. Walk the AST.
2. Evaluate innermost operation first (order of operations).
3. Record each mini-computation step.
4. Repeat until you have the final number.

**Equations:**
1. Move variables to one side, constants to the other.
2. Simplify both sides after each move.
3. Solve according to equation type (linear/quadratic).
4. Record each step in human-readable form.

---

## 5. Output Formatting
**Goal:** Make it look nice (Markdown).

**Tasks:**
- Add headings (`## Step 1`) for major steps.
- Use code blocks (```text) for steps.
- Highlight final answer (bold or backticks).
- Keep steps short but clear.

---

## 6. Optional Extras
- Validation — check for division by zero, malformed inputs, unsupported features.
- Multiple formats — Markdown, plain text, LaTeX.
- UI layer — CLI, Web, Mobile.
- Library mode — allow programmatic use.
- History tracking — store old problems.

---

## Mind-map Overview
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

---

## Components to Build
1. Lexer (token splitter)
2. Parser (AST builder)
3. Solver engine (rules + steps)
4. Formatter (Markdown output)
5. (Optional) Front-end (web/cli)
