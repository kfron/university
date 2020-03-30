package com.coldproject.DwarvenTreasure.States;

import java.awt.image.BufferedImage;
import java.awt.Graphics2D;
import java.awt.Color;

import javax.imageio.ImageIO;

import com.coldproject.DwarvenTreasure.Main.GamePanel;
import com.coldproject.DwarvenTreasure.Management.GSManager;
import com.coldproject.DwarvenTreasure.Management.KeysHandler;

public class IntroState extends GameState
{

	private BufferedImage logo;

	private int delta;
	private int ticks;

	private final int FADE_IN = 60;
	private final int LENGTH = 60;
	private final int FADE_OUT = 60;

	public IntroState(GSManager gsm)
	{
		super(gsm);
	}

	public void init()
	{
		ticks = 0;
		try
		{
			logo = ImageIO.read(getClass().getResourceAsStream("logo.png"));
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public void update()
	{
		processInput();
		ticks++;
		if(ticks < FADE_IN)
		{
			delta = (int) (255 - 255 * (1.0 * ticks / FADE_IN));
			if(delta < 0) delta = 0;
		}

		if(ticks > FADE_IN + LENGTH)
		{
			delta = (int) (255 * (1.0 * (ticks - FADE_IN - LENGTH) / FADE_OUT));
			if(delta > 255) delta = 255;
		}
		if(ticks > FADE_IN + LENGTH + FADE_OUT)
		{
			gsm.setState(GSManager.MENU);
		}
	}

	public void draw(Graphics2D g)
	{
		g.setColor(Color.WHITE);
		g.fillRect(0, 0, GamePanel.WIDTH, GamePanel.FULLHEIGHT);
		g.drawImage(logo, 0, 0, GamePanel.WIDTH, GamePanel.FULLHEIGHT, null);
		g.setColor(new Color(0,0,0,delta));
		g.fillRect(0,0,GamePanel.WIDTH,GamePanel.FULLHEIGHT);
	}

	public void processInput()
	{
		if(KeysHandler.isAnyKeyPressed())
		{
			gsm.setState(GSManager.MENU);
		}
	}
}