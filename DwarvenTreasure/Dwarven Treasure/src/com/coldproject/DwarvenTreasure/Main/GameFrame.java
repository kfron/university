package com.coldproject.DwarvenTreasure.Main;

import javax.swing.JFrame;
public class GameFrame extends JFrame
{
	GameFrame()
	{
		super("Dwarven Treasure");

		GamePanel gPanel = new GamePanel();
		add(gPanel);
		pack();

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLocationRelativeTo(null);
		setResizable(false);
		setVisible(true);
	}
}