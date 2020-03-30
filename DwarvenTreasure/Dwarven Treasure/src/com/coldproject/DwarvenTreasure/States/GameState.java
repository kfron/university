package com.coldproject.DwarvenTreasure.States;

import java.awt.Graphics2D;

import com.coldproject.DwarvenTreasure.Management.GSManager;

public abstract class GameState
{
	protected GSManager gsm;

	public GameState(GSManager gsm)
	{
		this.gsm = gsm;
	}

	public abstract void init();
	public abstract void update();
	public abstract void draw(Graphics2D g);
	public abstract void processInput();
}