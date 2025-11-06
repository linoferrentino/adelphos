<?php

namespace App\Data;

/*
 * Adelphos Backend, the abstract interface to the adelphos backend
 * which the web site will use.
 *
 * Copyright (c) 2025 Lino Ferrentino, lino.ferrentino@gmail.com
 *
 * GPL3 or above.
 */

class CurrencyCollection {


	// the array of currencies.
	public array $currencies;


	public function __construct() {


		$usd = new CurrencyData(1, "Dollar", "$", "USD", 1.0);
		$euro = new CurrencyData(2, "Euro", "€", "EUR", 1.0);
		$gbp = new CurrencyData(3, "British Pound", "£", "GBP", 0.76);
		$yen = new CurrencyData(4, "Yen", "¥", "JPY", 153.15);
		$chf = new CurrencyData(5, "Swiss Franch", "₣", "JPY", 0.81);

		$this->currencies = [
			$usd,
			$euro,
			$gbp,
			$yen,
			$chf
		];
	}

}


?>
