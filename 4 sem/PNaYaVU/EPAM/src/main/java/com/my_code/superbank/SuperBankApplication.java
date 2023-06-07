package com.my_code.superbank;
// подключили Lombok, чтобы не прописывать геттеры и сеттеры
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class SuperBankApplication { // точка старта
	public static void main(String[] args) {
		SpringApplication.run(SuperBankApplication.class, args);
	}
}