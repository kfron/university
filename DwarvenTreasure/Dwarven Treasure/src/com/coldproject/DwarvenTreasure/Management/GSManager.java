package com.coldproject.DwarvenTreasure.Management;

import java.awt.Graphics2D;

import com.coldproject.DwarvenTreasure.States.GameState;
import com.coldproject.DwarvenTreasure.States.IntroState;
import com.coldproject.DwarvenTreasure.States.MenuState;
import com.coldproject.DwarvenTreasure.States.PlayState;
import com.coldproject.DwarvenTreasure.States.GameOverState;

public class GSManager
{

	private int currState;
	private int prevState;
	private GameState[] gameStates;

	public static final int STATES_NUM = 4;
	public static final int INTRO = 0;
	public static final int MENU = 1;
	public static final int PLAY = 2;
	public static final int OVER = 3;

	public GSManager()
	{
		gameStates = new GameState[STATES_NUM];
		setState(INTRO);
	}

	public void setState(int i)
	{
		prevState = currState;
		gameStates[prevState] = null;
		currState = i;

		switch(i)
		{
			case INTRO:
			gameStates[i] = new IntroState(this);
			gameStates[i].init();
			break;
			case MENU:
			gameStates[i] = new MenuState(this);
			gameStates[i].init();
			break;
			case PLAY:
			gameStates[i] = new PlayState(this);
			gameStates[i].init();
			break;
			case OVER:
			gameStates[i] = new GameOverState(this);
			gameStates[i].init();
			break;
		}
	}

	public void update()
	{
		gameStates[currState].update();
	}

	public void draw(Graphics2D g)
	{
		gameStates[currState].draw(g);
	}
}