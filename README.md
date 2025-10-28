

# Adelphos: The Self-Organizing, Multi-Level Trust Network

> 40 καὶ ἀποκριθεὶς ὁ βασιλεὺς ἐρεῖ αὐτοῖς, Ἀμὴν λέγω ὑμῖν, ἐφ' ὅσον ἐποιήσατε ἑνὶ τούτων τῶν ἀδελφῶν μου τῶν ἐλαχίστων, ἐμοὶ ἐποιήσατε.  
>  
> The King will answer them, ‘Most certainly I tell you, because you did it to one of the least of these my brothers, you did it to me.’  
> <cite>- Matthew 25:40</cite>

## Table of Contents
- [What is Adelphos?](#what-is-adelphos)
- [Differences from Systems like LETS or Ripple](#differences-from-systems-like-lets-or-ripple)
- [Economy and Trust Within Families](#economy-and-trust-within-families)
- [Level Zero (L0): The Family Unit](#level-zero-l0-the-family-unit)
- [Level One (L1): The Cohort](#level-one-l1-the-cohort)
- [Self-Organizing Nature of Adelphos](#self-organizing-nature-of-adelphos)
- [Not a Ponzi Scheme](#not-a-ponzi-scheme)
- [Measuring Trust in Adelphos](#measuring-trust-in-adelphos)
- [Money in Adelphos](#money-in-adelphos)
- [License](#license)

## What is Adelphos?
Adelphos (from the ancient Greek word for "brother") is a system that enables users across regions or countries to exchange goods and services using a hybrid of real currency (cash or deferred payments) and virtual credit. It builds on natural human trust starting from small groups like families, scaling fractally to larger structures like neighborhoods or communities. This creates a self-organizing network where trust grows organically, reducing reliance on traditional banking.

## Differences from Systems like LETS or Ripple
- **LETS (Local Exchange Trading Systems)**: Uses purely virtual credits that cannot leave the system, limiting flexibility for real-world needs.
- **Ripple**: Relies on a mesh of interconnections for credit paths, but lacks Adelphos' fractal hierarchy, which mimics social structures for more intuitive scaling and risk management.

## 🏡 Economy and Trust Within Families
Families form the core of human trust and economy, regardless of culture. Key traits include:
- **Mutual trust**: Members rely on each other without constant tracking.
- **Shared responsibility**: Debts or obligations are collective.

Examples:
- Parents cover a child's mistakes (e.g., breaking a window).
- Spouses share financial liabilities, even with separate assets.

These principles underpin Adelphos' **Level Zero (L0) Trust**.

## 👨‍👩‍👧‍👦 Level Zero (L0): The Family Unit
In Adelphos, an L0 group isn't limited to biological families—it's any small unit acting as one economically.

Characteristics:
- **Shared ledger**: No individual accounts; all transactions are group-wide.
- **Full mutual responsibility**: Members are liable for all debts, including Redeemable Money (RM) and Unredeemable Money (UM).
- **External view**: The group appears as a single entity to outsiders.

Each group enters with a fixed equity (risk limit), capping total debt to prevent overextension.

## 🧑‍🤝‍🧑 Level One (L1): The Cohort
L1 groups represent close friends or trusted allies—people you trust deeply but don't share daily finances with.

Key differences from L0:
- No shared living or joint accounts.
- Trust is limited: Informal for small exchanges, formal (e.g., IOUs) for larger ones.

Example:
> Alice and Lucy are close friends. They exchange small gifts without tracking debts. But if Lucy sells school books to Alice for $200, she expects cash or a formal IOU.

In Adelphos, L1 cohorts pool equities from L0 groups, exposing a portion (e.g., 50%) externally while maintaining internal trust.

## 🧬 Self-Organizing Nature of Adelphos
Adelphos grows fractally: Higher levels (e.g., L2 neighborhoods) mimic lower ones, with admins, pooled equities, and shared responsibilities. This allows organic expansion from families to communities, using routing like DNS for efficient credit paths (O(log n) complexity in balanced trees).

## 🚫 Not a Ponzi Scheme
Adelphos is the opposite of a Ponzi scheme. In Ponzi models, the top exploits the bottom without responsibility. Here, higher levels are liable for lower ones' debts, distributing risk upward and encouraging accountability.

## 🤝 Measuring Trust in Adelphos
Trust defies simple definition but, in Adelphos, it's the credit you're willing to extend—quantified by amount and time.

### 💡 What Is Trust?
If Alice sells Bob a $300 phone and accepts a 30-day IOU, she's trusting him for $300 over that period. Longer delays may include interest (e.g., $310 after a year for 3%).

### 📐 Trust as a Measurable Concept
Trust is the delayed payment you're willing to accept. Example:
> Selling a €4,000 car to a friend for €3,500 cash + €500 IOU yields a 12.5% trust ratio.

Adelphos records IOUs, making trust quantifiable and relational.

### 🧭 Philosophy of Trust
Adelphos shifts from scarcity-based money to credit-based exchange without fully replacing currencies. Users set personal trust thresholds to limit risk.

### 📊 The Axiom of Adelphos
One unit of trust equals one unit of currency exchanged. We denote currency as τ (Tao).

### 🧮 The Human Unit
Currencies vary; the "human unit" (τ_HV) is the smallest meaningful amount (e.g., $1 in the US, 1,000 pre-Euro Lire in Italy).

### 📏 The Trust-bel: Unit of Trust
Trust is measured in trust-bel (tb):
```
trust-bel = 10 * log10(τ / τ_HV)
```
Where τ is postponed currency, τ_HV is the human unit.

Example 1 (USA):
> Alice lends Bob $100 (τ_HV = $1).  
> tb = 10 * log10(100 / 1) = 20 tb.

Example 2 (pre-Euro Italy):
> Anna lends Bruno 1,000,000 Lire (τ_HV = 1,000 Lire).  
> tb = 10 * log10(1,000,000 / 1,000) = 30 tb.

The logarithmic scale reflects how trust grows non-linearly.

### 🧠 Trust in Practice
Trust-bel standardizes comparisons across currencies (e.g., using USD exchange rates for Yen). It's used for credit limits and routing.

## 💰 Money in Adelphos
Money is either real (external cash) or deferred (internal credit).

Deferred types:
- **Redeemable Money (RM)**: IOU with a due date; may accrue interest.
- **Unredeemable Money (UM)**: Credit without a repayment date; lower trust typically.

### 🔁 Redeemable Money (RM)
Users set RM trust levels, limiting acceptance.

Example:
> Alice buys a $300 phone from Bob (20 tb trust).  
> RM_max = 10^(20 / 10) * $1 = $100.  
> She pays $200 cash + $100 RM IOU.

RM is spendable within limits.

Further Example:
> Bob uses $60 RM for Lucy's lessons (she accepts up to $100). He has $40 left.

Observations:
- Trust is relational (Lucy trusts the IOU issuer).
- Credit accumulates; exceeding limits requires cash.

### 🌀 Unredeemable Money (UM)
Similar to RM but indefinite. Users set separate (often lower) trust levels.

Key Differences:
- RM may include interest; UM does not.
- Users are fully liable for their own RM/UM, but only RM from invited friends.

## License
Copyright (c) Lino Ferrentino 2025. This project is licensed under the GPLv3 License.

---

