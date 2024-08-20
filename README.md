# ABS Validation with DLAS
I aimed to validate the ABS (Anti-lock Braking System) using AI. For this validation, I utilized DLAS, an improved version of the well-known hill climbing method. 

I built the model based on the slip ratio-friction coefficient formula:
$$\mu=1.2sin[1.65tan^{-1}\lbrace12s-tan^{-1}(6s)\rbrace]$$

The following graph represents the results obtained using this formula:

![image](https://github.com/user-attachments/assets/043db65b-2dbb-4f14-ab06-353e1f3da5ed)

The resulting model closely matched the ABS system. The friction ratio in the result model remained at approximately 90% of the highest value observed in the graph.

## Reference
Namazi, M., Sanderson, C., Newton, M. A. H., Polash, M. M. A., & Sattar, A. (2018). Diversified Late Acceptance Search. In *Lecture Notes in Computer Science* (pp. 299–311). https://doi.org/10.1007/978-3-030-03991-2_29
