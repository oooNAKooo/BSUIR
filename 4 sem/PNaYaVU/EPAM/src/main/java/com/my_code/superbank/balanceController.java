package com.my_code.superbank;

import com.my_code.superbank.model.BalanceResponse;
import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.math.BigDecimal;

// когда происходит запуск проги и пока будет идти скан пакетов
// она поймет что нужно вложить данный экземпляр класса в контекст
// @Controller=@Service=@Repository=@Component (@Component - основной)
// @Controller=@RestController
//@RequestMapping
@Slf4j // экземпляр loggera
@RestController("/balance")
@AllArgsConstructor // создание конструкторов для final полей
public class balanceController {

    private final bankService bankService;

    @GetMapping("/{accountID}") // проверка денег на счете
    public BigDecimal getBalance(@PathVariable Long accountID){
        return bankService.getBalance(accountID);
    }

    @PostMapping("/add")
    public ResponseEntity<BalanceResponse> addMoney(@RequestParam("to") Long to,
                                                    @RequestParam("amount")BigDecimal amount) {
        BigDecimal totalAmount = BigDecimal.ZERO;
        BigDecimal updatedBalance = bankService.addMoney(to, totalAmount);
        BalanceResponse response = new BalanceResponse(updatedBalance);
        return ResponseEntity.ok(response);
    }

    @ExceptionHandler(RuntimeException.class) // возврат ошибки 500
    public ResponseEntity<String> handleInternalError(RuntimeException e) {
        log.error("Internal error occurred", e);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body("Internal error occurred");
    }
    @GetMapping("/divide/{accountID}")
    public ResponseEntity<BalanceResponse> divideBalance(@PathVariable Long accountID) {
        BigDecimal result = bankService.divideBalance(accountID);
        BalanceResponse response = new BalanceResponse(result);
        return ResponseEntity.ok(response);
    }
}