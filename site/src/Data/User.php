<?php

namespace App\Data;


/*
 *
 * A user in adelphos.
 * just a read only class
 *
 */
readonly class UserData {

	public function __construct(
		public int $id,
		public string $name,
		public string $family_name,
		public string $email
	}


}


