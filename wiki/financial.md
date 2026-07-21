# Component: Financial

## Purpose

Financial handles money topics: not affording something, debt, losing a job,
saving toward a goal, investing, spending, and general money worry. It sits
third in the dispatch order, after `concern` and `relationships`, so a line
that's really about crisis or a partner gets claimed by those first. "I hate
my job" goes to `concern` and "I love my job" goes to `relationships`, even
though both mention a financial keyword — that's the intended result of the
ordering.

## Data storage

No state persists between calls. `financialMatches()` and
`financialRespond()` are free functions declared in `financial.h` and wired
into the `Category` table in `category_registry.cpp`. `respond()` declares
nine `static const std::regex` objects that compile once on first use and
get reused after that, the same pattern the other implemented components
follow.

## Functionality

`financialMatches()` runs one `regex_search` against a keyword alternation
covering money, debt, owe, loan, save/savings, invest, bankrupt, broke,
afford, rent, bills, paycheck, salary, income, budget, spend/spent,
expensive, cost, credit, mortgage, job, and laid off.

`financialRespond()` checks the most specific patterns first. A bare "I'm
broke" is caught with `regex_match` against the whole line, so it doesn't
get mixed up with longer sentences that happen to contain the word. After
that come "I can't afford X," job loss ("I lost my job"/"I got laid
off"/"I got fired"), debt, "I owe X," saving toward a goal, investing,
"I spent X," and "I'm worried about X." Each of the capturing patterns pulls
the object out with a group, runs it through `text_utils::reflect` so the
pronouns flip, and builds the reply with `text_utils::fillTemplate`.

The saving pattern is written as two alternatives ("I'm saving up for X" and
"I want to save for X") with a capture group in each, so the code checks
`match[1].matched` to decide which group actually holds the goal. If the
keyword check passed but nothing more specific fired — a bare mention of
"credit" or "budget," say — it falls back to a general prompt.

Dollar amounts are worth noting: because `fillTemplate` builds the reply
with `regex_replace`, a captured `$` would otherwise be read as a group
reference and mangle the output. `text_utils::fillTemplate` escapes it, so
"I spent $50 on groceries" correctly comes back as "How did you feel after
spending $50 on groceries?"
