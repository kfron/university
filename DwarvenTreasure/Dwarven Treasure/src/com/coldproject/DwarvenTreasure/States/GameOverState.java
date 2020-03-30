package com.coldproject.DwarvenTreasure.States;

import java.awt.Color;
import java.awt.Graphics2D;

import com.coldproject.DwarvenTreasure.Main.GamePanel;
import com.coldproject.DwarvenTreasure.Management.GSManager;
import com.coldproject.DwarvenTreasure.Management.KeysHandler;
import com.coldproject.DwarvenTreasure.Management.Graphics;

public class GameOverState extends GameState
{
	private Color color;

	public GameOverState(GSManager gsm)
	{
		super(gsm);
	}

	public void init()
	{
		color = new Color(164, 198, 222);
	}

	public void update() 
	{
		processInput();
	}

	public void draw(Graphics2D g)
	{
		g.setColor(color);
		g.fillRect(0,0,GamePanel.WIDTH,GamePanel.FULLHEIGHT);

		Graphics.drawString(g, "Thanks", 40, 36);
		Graphics.drawString(g, "for playing the", 4, 44);
		Graphics.drawString(g, "alpha version of", 0, 52);
		Graphics.drawString(g, "my game", 36, 60);
		Graphics.drawString(g, "press any key", 12, 110);
	}

	public void processInput()
	{
		if(KeysHandler.isDown(KeysHandler.ENTER))
			gsm.setState(GSManager.MENU);
	}
}