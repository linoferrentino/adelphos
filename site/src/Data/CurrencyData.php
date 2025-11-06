<?php

namespace \App\Data;


/*
 *
 * A Currency for adelphos is composed by a name (such as dollar)
 * a symbol (maybe $ or €)
 * a human value (which is roughly the amount of one dollar)
 * a symbol in the forex exchange like (USD)
 *
 */
class CurrencyData {

	private function __construct(
		public string $name,
		public string $symbol,
		public string $forex_string,
		public double $human_value) {
	
	}


}


