package com.coldproject.DwarvenTreasure.Main;

import javax.swing.JPanel;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import com.coldproject.DwarvenTreasure.Management.KeysHandler;
import com.coldproject.DwarvenTreasure.Management.GSManager;

public class GamePanel extends JPanel implements Runnable, KeyListener
{
	//wymiary jako stałe
	public static final int WIDTH = 128;
	public static final int HEIGHT = 128;
	public static final int FULLHEIGHT = HEIGHT + 16;
	public static final int SCALE = 3;

	//thread
	private Thread thread;
	private boolean isRunning;
	private final int FPS = 30;
	private final int TARGET_TIME = 1000/FPS;

	//rysowanie
	//przechowuje grafike
	private BufferedImage img;
	//interfejs do renderowania w ogólności
	private Graphics2D g;
	//jak ja to rozumiem: BufferedImage to "obraz", a Graphics2D to "artysta" :)


	private GSManager gsman;


	public GamePanel()
	{
		setPreferredSize(new Dimension(WIDTH * SCALE, FULLHEIGHT * SCALE));
		setFocusable(true);
		requestFocus();

	}

	public void addNotify()
	{
		super.addNotify();
		if(thread == null)
		{
			addKeyListener(this);
			thread = new Thread(this);
			//uruchamia run();
			thread.start();
		}
	}

	public void run()
	{
		init();
		// startTime i elapsedTime obliczają czas, który zajęło wykonanie jednej sekwencji pętli
		// waitTime jest wykorzystywany, aby gra nie była "za szybka".
		long startTime, elapsedTime, waitTime;
		//game loop
		while(isRunning)
		{
			startTime = System.nanoTime();

			//sekwencja pętli
			processInput();
			draw();
			paintComponent();

			elapsedTime = System.nanoTime() - startTime;


			waitTime = TARGET_TIME - elapsedTime / 1000000;
			if (waitTime < 0)
			{
				waitTime = TARGET_TIME;
			}

			try
			{
				Thread.sleep(waitTime);
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}
	}

	private void init()
	{
		isRunning = true;
		img = new BufferedImage(WIDTH, FULLHEIGHT, 1);
		//zwraca kontekst dla g, ale typu Graphics, stąd cast
		g = (Graphics2D) img.getGraphics();
		gsman = new GSManager();
	}

	//przewtarzanie inputu - "odświeżanie"
	public void processInput()
	{
		gsman.update();
		KeysHandler.processInput();
	}

	// stworzenie obrazu w pamięci, w BufferedImage z nim powiązanym
	private void draw()
	{
		gsman.draw(g);
	}

	private void paintComponent()
	{
		//wyciąga zawieszony kontekst
		Graphics2D g2 = (Graphics2D) getGraphics();
		g2.drawImage(img, 0, 0, WIDTH*SCALE, FULLHEIGHT * SCALE, null);
		//zwolnienie zasobów
		g2.dispose();
	}

	public void keyPressed(KeyEvent event)
	{
		KeysHandler.keySet(event.getKeyCode(), true);
	}

	public void keyReleased(KeyEvent event)
	{
		KeysHandler.keySet(event.getKeyCode(), false);
	}
	public void keyTyped(KeyEvent key) {}


}