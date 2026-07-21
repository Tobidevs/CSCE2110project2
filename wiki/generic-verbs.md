# Component: Generic Verbs

## Purpose

Generic Verbs handles general statements built around common everyday verbs:
having something, buying or wanting to buy something, traveling, knowing
something, and learning something. This was the Milestone 1 component, so it
was also the piece that proved out the shape every later component copied — a
keyword gate paired with an ordered list of response patterns.

It sits last in the dispatch order, and that placement is the whole point.
Its keyword net is deliberately the widest in the program, so if it ran
earlier it would swallow lines that belong to a more specific category. "I
have a lot of debt" and "I have an exam tomorrow" both contain "have," but
`financial` and `education` sit ahead of this component and claim them
first. Generic Verbs only ever sees what nothing else wanted.

## Data storage

No state persists between calls. `genericVerbsMatches()` and
`genericVerbsRespond()` are free functions declared in `generic_verbs.h` and
registered into the shared `Category` table that `category_registry.cpp`
builds. `respond()` declares ten `static const std::regex` objects, which are
compiled once the first time the function runs and reused on every later
call rather than being rebuilt per line.

Each pattern is paired with a response template containing an uppercase
placeholder such as `OBJECT`, `PLACE`, or `TOPIC`. Captured text is passed
through `text_utils::reflect()` before being substituted, which is what turns
"I have my keys" into "How long have you had your keys?" The substitution
itself goes through `text_utils::fillTemplate()`, a helper that started out
local to this file and was later moved into `text_utils` once the other
components needed the same behavior.

## Functionality

`genericVerbsMatches()` runs a single `regex_search` against an alternation of
have/has/had, the purchase family (purchase, bought, buy), the travel family,
and know/knew/known/learn, and returns true if any of them appear.

`genericVerbsRespond()` checks patterns in order of specificity rather than in
whatever order they were written. A bare "I don't know" — the kind of short
answer the assignment specifically calls out — is caught first with
`regex_match` against the whole trimmed line, so it gets a reply about taking
your time rather than being treated as a sentence with an object. The same
phrase appearing mid-sentence is caught separately afterward with
`regex_search` and answered differently.

The remaining patterns cover "I don't have X" and "I have X" (negation
checked first so it isn't swallowed by the plain rule), wanting to purchase
versus having purchased something, traveling somewhere, learning a topic, and
knowing something. Each captures its object with a group, reflects it, and
builds the reply from a template.

Input is the raw line typed by the user, passed in unmodified by
`conversation::run()`. Output is a single string, which the caller prints. If
the keyword check passed but no specific pattern fired, the function returns
a general acknowledgment rather than an empty string — every category does
this, and together with the program-wide fallback it is what guarantees the
user always gets some response.
