# Blackjack probabilistic calculator
A simple calculator that evaluates the chances of winning a given position in blackjack. Due to the fact that _double down_, _split_ and _insurance_ are purely gambling decisions based on one's own sense of luck (or they come down to the basic _hit_ and _stand_), the project calculates the probability of winning if we _stand_ or if we _hit_.
## The method
### Randomizing games after _hitting_
Game simulation after a _hit_ decision requires drawing a future decision (_hitting_ or _standing_). For this reason, the odds are defined as follows:
- 4 to 11 points - 100%
- 12 points - 64%
- 13 points - 57%
- 14 points - 50%
- 15 points - 43%
- 16 points - 36%
- 17 points - 29%
- 18 points - 22%
- 19 points - 14%
- 20 points - 8%
- 21 points - 0%

This is due to the fact that for points 4-11, no card drawn will end the game, for points 12, 9 out of a possible 14 cards will not end the game, etc.
Randomly generated decisions (50/50) do not provide measurable results because they omit the element of mathematical possibility of calculating the final result.
### Randomizing games after _standing_
Game simulation after a _stand_ decision does not require randomizing player decisions. As long as dealer has below 17 points he must draw another card. Otherwise, he must stop. Thus player can win only by having more score or by dealer overflowing his deck.
## Result interpretation
After the simulation, percentage results are presented, indicating how many games on average you would win after making a given decision in this very position.