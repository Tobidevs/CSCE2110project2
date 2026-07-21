# Component: Concern

## Purpose

Concern watches for crisis-adjacent language in the user's input: hate,
self-directed anger, hopelessness, and anything that sounds like suicidal or
self-harm intent. It's checked before every other category so a line like
"I want to kill myself" gets caught here instead of being picked up by a
broader category that would respond to it as an ordinary statement. Because
this is the category the assignment calls out as needing safe, careful
responses, its replies stay supportive and, for the more serious language,
point to the 988 Suicide & Crisis Lifeline instead of just deflecting.

## Data storage

Concern doesn't hold any state between calls. Like the other categories, it's
a `matches()`/`respond()` pair of free functions declared in `concern.h` and
added to the `Category` table in `category_registry.cpp`. Inside
`concernRespond()`, six `static const std::regex` objects are built the
first time the function runs and reused after that, so the patterns only get
compiled once. The supportive/resource reply text is pulled into one string
constant, `kCrisisResponse`, so both of the self-harm patterns return the
exact same wording.

## Functionality

`concernMatches()` does a single `regex_search` against a keyword
alternation: stems like `suicide`, `kill`, `hate`, `harm`, `hurt`,
`hopeless`, `worthless`, plus phrases like "no reason to live." It's meant
to be wide, since a false positive here just gets a gentle follow-up
question, while a missed line is a bigger problem.

`concernRespond()` checks patterns in order of how serious they are, not in
whatever order they happen to sit in the file. Two patterns look for
self-harm or suicide language, one for the whole line matching exactly
(`regex_match`) and one for the same phrasing showing up mid-sentence
(`regex_search`); both return `kCrisisResponse`. After that it checks
self-directed hate ("I hate myself"), then named feelings ("I feel
hopeless"/"I feel worthless"), pulling the matched word out with a capture
group and echoing it back through `text_utils::fillTemplate`. The last two
patterns catch hate or aggression aimed at something other than the
speaker, and reflect the object the same way `generic_verbs` does, so "I
hate my job" turns into "What is it about your job that troubles you so
much?" If the keyword check passed but nothing else matched, it falls back
to a generic but still supportive line.

Since `concern` runs first in `category_registry.cpp`, something like "I
hate my husband" ends up here instead of in `relationships`, even though it
also matches that category's keywords. That's intentional — this category
exists to catch emotionally loaded language before anything else gets a
chance to treat it lightly.
