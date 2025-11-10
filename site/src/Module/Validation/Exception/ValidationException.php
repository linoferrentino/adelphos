<?php

namespace App\Module\Validation\Exception;

use RuntimeException;

class ValidationException extends RuntimeException
{
    public readonly array $validationErrors;

    public function __construct(array $validationErrors, string $message = 'Validation error')
    {
        parent::__construct($message);

        $this->validationErrors = $this->transformCakephpValidationErrorsToOutputFormat($validationErrors);
    }

    /**
     * Transform the validation error output from the library to array that is used by the frontend.
     * 
     * Removes the rule name as keys and only keeps the error message.
     *
     * @param array $validationErrors The cakephp validation errors
     *
     * @return array the transformed result
     */
    private function transformCakephpValidationErrorsToOutputFormat(array $validationErrors): array
    {
        $validationErrorsForOutput = [];
        foreach ($validationErrors as $fieldName => $fieldErrors) {
            // There may be cases with multiple error messages for a single field
            foreach ($fieldErrors as $infringedRuleName => $infringedRuleMessage) {
                // Removes the rule name as keys and replace with incremented numeric keys
                $validationErrorsForOutput[$fieldName][] = $infringedRuleMessage;
            }
        }

        return $validationErrorsForOutput;
    }
}
