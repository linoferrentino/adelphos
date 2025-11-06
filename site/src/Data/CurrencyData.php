<?php

namespace App\Data;


/*
 *
 * A Currency for adelphos is composed by a name (such as dollar)
 * a symbol (maybe $ or €)
 * a human value (which is roughly the amount of one dollar)
 * a symbol in the forex exchange like (USD)
 *
 */
readonly class CurrencyData {

	public function __construct(
		public int $id,
		public string $name,
		public string $symbol,
		public string $forex_string,
		public float $human_value) {
	
	}


}


