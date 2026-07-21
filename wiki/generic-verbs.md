# Component: Generic Verbs

## Purpose

Handles general statements built around common verbs: having something, buying or wanting to buy something, traveling, knowing something, and learning something. This was the Milestone 1 component.

## Data storage

No persistent state. Nine `static const std::regex` objects are declared inside `genericVerbsRespond()`, compiled once and reused on every call. Each is paired with a response template; captured text is passed through `text_utils::reflect()` before being inserted, so "I have my keys" becomes "How long have you had your keys?"

## Functionality

`genericVerbsMatches()` runs one `regex_search` against an alternation of have/has/had, purchase-family, travel-family, and know/learn-family keywords to decide if this category should handle the line.

`genericVerbsRespond()` checks patterns in order of specificity. A bare "I don't know" is caught with `regex_match` against the whole line; the same phrase mid-sentence is caught separately with `regex_search`. The remaining patterns cover I have/don't have X, want to purchase/purchased X, traveled to X, know X, and learned X — each using `regex_search` to capture the object and `regex_replace` to build the reply from a template. If the keyword check passed but no specific pattern fired, it returns a generic acknowledgment.

Input is the raw line typed by the user; output is a single string printed by `conversation::run()`.
