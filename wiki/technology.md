# Component: Technology & Social Media

## Purpose

Technology is the self-defined category the assignment allows each group to
add on top of the seven it names. It covers phones and computers, the
internet, apps and social media, screen time, scrolling, video games, and
devices breaking. It made sense as the eighth category because so much of
what someone would bring to a conversation like this now involves a screen,
and none of the seven required categories has a natural place for it.

It sits seventh, immediately before `generic_verbs`. That position matters:
it's specific enough that it shouldn't be intercepting lines meant for
health or money, but it still needs to run before the catch-all.

## Data storage

No state persists between calls. `technologyMatches()` and
`technologyRespond()` are free functions declared in `technology.h` and
registered in the `Category` table like every other component. `respond()`
declares nine `static const std::regex` objects, compiled once on first use
and reused after that.

## Functionality

`technologyMatches()` runs one `regex_search` against a keyword alternation
covering phone, computer, laptop, tablet, internet, wifi, apps, social media
and the individual platform names, screen time, online, scrolling, texting,
email, games, gaming, and notifications.

`technologyRespond()` goes from specific to general: spending too much time
on something, being addicted to something, not being able to put a phone
down, screen time totals, scrolling, deleting or quitting an app, playing
games, getting a new device, and a device breaking. Captured text runs
through `text_utils::reflect` and is substituted with
`text_utils::fillTemplate`.

Two of these patterns exist because of how the ordering interacts with other
components. The first is "I spend too much time on X" — `financial` sits
ahead of this component and originally matched the bare word "spend," which
meant a line about phone use was being answered as though it were about
money. Financial's keyword list was narrowed to "spent" and "spending" so
that stopped happening; a line like "I spend money on apps" still reaches
`financial` because it contains the word "money."

The second is the new-device pattern. Because this component owns the device
nouns, "I have a new laptop" matches here rather than falling through to
`generic_verbs`, which used to answer it. Without a pattern of its own the
line would only have gotten this component's generic fallback, so
`technologyRespond()` handles acquiring a device directly and asks how the
user is finding it. This is the general lesson of the dispatch order: once a
component claims a keyword, it takes responsibility for every phrasing that
keyword appears in.
