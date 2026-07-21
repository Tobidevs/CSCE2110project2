# myELIZA — Architecture Notes

Working notes for the Week 1 design pass. These map to the three sections of
the course's Project Design Document (Functionality Needs, Data Storage, File
Organization) and are the source material for the wiki's high-level page
later — not a replacement for it.

## Components and priority order

Eight topic components, each a `matches()` / `respond()` pair registered in
`category_registry.cpp`, checked in this order against each line of input:

1. `concern` — hate, kill, suicide, and other crisis-adjacent language
2. `relationships` — love, spouse, girlfriend, boyfriend
3. `financial` — money, savings, investments, bankruptcy
4. `wellbeing` — exercise, vitamins, relaxing, sleeping
5. `education` — courses, university, studying, student
6. `entertainment` — concert, movies, dancing, restaurant, bar
7. `technology` — phone, internet, apps, screen time, social media (self-defined)
8. `generic_verbs` — have, purchase, travel, know, learn

The first category whose `matches()` returns true handles the line; the rest
are never consulted. Order is priority, not build order: `concern` is first
so crisis language can never be caught by a broader category's regex before
the safety-specific response fires, and `generic_verbs` is last because it's
deliberately the widest net and should only catch what nothing else claimed.

## Functionality needs

Each component owner is responsible for:
- One `matches(input)` function using `regex_match`/`regex_search` to decide
  if the category applies.
- One `respond(input)` function using `regex_replace` (often together with
  the shared `text_utils::reflect` pronoun swap) to build the reply.
- Enough regexes in that category to comfortably clear the 50-total minimum
  once all 8 are built (roughly 6-7 each).

Cross-cutting functionality lives outside any single component:
- `text_utils::normalize` — lowercases and trims input/punctuation so the
  response tracker and the exit check ("bye") compare consistently.
- `text_utils::reflect` — the classic ELIZA pronoun swap ("I" → "you", "my" →
  "your"), done as a single word-by-word pass specifically so a string
  doesn't get swapped twice in the same pass. Any component can call it when
  building a reflected reply.
- `conversation::run` — the only place that sequences things: normalize →
  check "bye" → check repeat → walk the category registry → fall back if
  nothing matched → record the response. No component calls another
  component directly; everything is sequenced through this loop.

Sequencing dependency: every component depends on `ResponseTracker` existing
and being populated before its own logic runs, which is why that was built in
Week 1 alongside the registry rather than left for whoever builds the first
component.

## Data storage

Two data structures are shared across the whole system rather than owned by
any one component:

- **`Category` records** (`category.h`): a name plus two function pointers.
  `category_registry.cpp` assembles these into a `std::vector<Category>` in
  priority order — a flat table rather than a class hierarchy, so adding a
  9th category later is one struct literal, not a new subclass wired into a
  virtual dispatch chain.
- **`ResponseTracker`**: an `unordered_map<string, int>` keyed on normalized
  input text, counting how many times each distinct answer has been given.
  A hash map was chosen over a list because the only question ever asked of
  it is "have I seen this exact normalized string before," which is an O(1)
  lookup either way — no ordering or history-of-history is required by the
  spec. It is intentionally *not* one map per category: "repeated or
  identical answers" in the spec reads as global (the user said the same
  thing twice), not per-topic.

Everything else (which regex matched, what a component's response text is)
stays local to that component's `.cpp` file and doesn't need to be shared.

## File organization

- `main.cpp` contains only `int main()`, which calls `conversation::run()`.
- Every other `.cpp` has a matching `.h`; only headers are ever `#include`d,
  never another `.cpp`.
- Each of the 8 components is a self-contained header/source pair with no
  dependency on any other component's header — they only depend on
  `category.h` (for the shared struct) and, optionally, `text_utils.h`.
- `category_registry.cpp` is the one file allowed to include every
  component's header, since assembling the table is its entire job.
- `conversation.cpp` depends on `category_registry.h`, `response_tracker.h`,
  and `text_utils.h`, but never on an individual component header directly —
  it only ever sees components through the `Category` interface.

This keeps the dependency direction one-way (`main` → `conversation` →
`category_registry` → individual components) so no component's build breaks
another's, which matters for merging component branches independently.

## Current status

`generic_verbs` (Milestone component #1), `concern`, and `relationships` are
implemented — 9, 7, and 8 regexes respectively, built with `regex_match`,
`regex_search`, and `regex_replace`. `fillTemplate` moved from a local helper
in `generic_verbs.cpp` to `text_utils` since all three components need it.
The remaining 5 components (`financial`, `wellbeing`, `education`,
`entertainment`, `technology`) are still stubbed. Program compiles and runs
end-to-end: greeting, per-category responses, fallback, repeat detection,
and "bye" exit all work.
