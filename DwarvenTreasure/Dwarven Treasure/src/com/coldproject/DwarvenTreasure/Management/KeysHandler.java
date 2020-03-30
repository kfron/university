package com.coldproject.DwarvenTreasure.Management;

import java.awt.event.KeyEvent;

// Odpowiada za stan wciśnięty klawiszy
// mapuje odpowiednie zachowania dla każdego z określonych przycisków.

// Za aktualizacje stanu odpowiada KeyListener z klasy GamePanel

//GamePanel KeyListener aktualizuje tablice.
public class KeysHandler
{
	public static final int KEYS_NUM = 6; // 6 obsługiwanych przycisków

	public static boolean prevkeysState[] = new boolean[KEYS_NUM];
	public static boolean keysState[] = new boolean[KEYS_NUM];

	public static int UP = 0;
	public static int DOWN = 1;
	public static int LEFT = 2;
	public static int RIGHT = 3;
	public static int SPACE = 4;
	public static int ENTER = 5;

	public static void keySet(int i, boolean b)
	{
		switch(i)
		{
			case KeyEvent.VK_UP:
			keysState[UP] = b;
			break;

			case KeyEvent.VK_DOWN:
			keysState[DOWN] = b;
			break;

			case KeyEvent.VK_LEFT:
			keysState[LEFT] = b;
			break;

			case KeyEvent.VK_RIGHT:
			keysState[RIGHT] = b;
			break;

			case KeyEvent.VK_SPACE:
			keysState[SPACE] = b;
			break;

			case KeyEvent.VK_ENTER:
			keysState[ENTER] = b;
			break;

		}
	}

	public static void processInput()
	{
		for(int i = 0; i < KEYS_NUM; i++)
			prevkeysState[i] = keysState[i];
	}

	public static boolean isPressed(int i) // czy został naciśnięty -> wcześniej nie był
	{
		return keysState[i] && !prevkeysState[i];
	}

	public static boolean isDown(int i) // czy jest wciśnięty
	{
		return keysState[i];
	}

	public static boolean isAnyKeyPressed()
	{
		for(int i = 0; i < KEYS_NUM; i++)
			if(isPressed(i)) return true;
		
		return false;
	}

	public static boolean isAnyKeyDown()
	{
		for(int i = 0; i < KEYS_NUM; i++)
			if(isDown(i)) return true;

		return false;
	}

}