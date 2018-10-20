package com.ca2;

import android.app.Activity;
import android.os.Bundle;
import android.os.Build;
import android.text.Selection;
import android.content.Context;
import android.view.View;
import android.widget.TextView;
import android.widget.EditText;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.text.Editable;
import android.text.SpannableStringBuilder;
import android.view.Display;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.text.InputType;
import android.view.inputmethod.EditorInfo;
import android.graphics.Point;
import android.view.MotionEvent.PointerProperties;
import android.content.pm.PackageManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.content.Intent;
import android.util.Log;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import android.app.WallpaperManager;
import android.media.AudioManager;


public class app extends Activity
{

	view			m_view;

	private static native void configureApp(String strCommandLine, String strCacheDir, int iScreenW, int iScreenH);
    
    @Override
    public void onCreate(Bundle savedInstanceState)
    {

        super.onCreate(savedInstanceState);

		Display display = getWindowManager().getDefaultDisplay();

		Point size = new Point();
		
		display.getSize(size);

		//configureApp("app : app=app-core/hellomultiverse no_hello_edit client_only", getApplicationContext().getCacheDir().getAbsolutePath(), size.x, size.y);

		String prjname = "";

		String cmdline = "";

		String libname = "";

		try
		{

			ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
    
			Bundle bundle = ai.metaData;
		
			prjname = bundle.getString("project_name");

			cmdline = bundle.getString("command_line");

			libname = bundle.getString("library_name");

		}
		catch (NameNotFoundException e)
		{
			
		}
		catch (NullPointerException e)
		{
			
		}



		if(prjname.substring(0, 5).equals("nord_"))
		{

			prjname = prjname.substring(5);

		}

        System.loadLibrary("activity");

		//System.loadLibrary("axis");

		//System.loadLibrary("core");

		//System.loadLibrary("axispixman");

		//System.loadLibrary("axiscairo");

		if(libname.length() > 0)
		{
			
			String[] parts = libname.split(",");

			for(int i = 0; i < parts.length; i++)
			{
				
				String lib = parts[i];

				if(lib.length() > 0)
				{

					System.loadLibrary(lib);

				}

			}

		}

		//System.loadLibrary("axis_image_jpeg");

		//System.loadLibrary("draw2d_cairo");

		boolean bLoaded = false;

		if(!bLoaded)
		{


			try
			{

				System.loadLibrary(prjname);

				bLoaded = true;

			}
			catch (UnsatisfiedLinkError e)
			{

			}
		
		}

		if(!bLoaded)
		{

			if(prjname.substring(0, 4).equals("app_"))
			{

				prjname = prjname.substring(4);

			}
			else
			{

				throw new UnsatisfiedLinkError(prjname);

			}
		
			//try
			{

				System.loadLibrary(prjname);

				bLoaded = true;

			}
			//catch (UnsatisfiedLinkError e)
			{

			}

		}

		System.loadLibrary("launcher");

		configureApp(cmdline, getApplicationContext().getCacheDir().getAbsolutePath(), size.x, size.y);

		m_view = new view(this, size);
		
        setContentView(m_view);

    }


}



class TakeInfoResult
{

   boolean	m_bShowKeyboard;

   boolean	m_bHideKeyboard;

   String	m_strOpenUrl;

   String	m_strWallpaper;
   
}


class view extends EditText implements View.OnKeyListener
{


	class MyInputConnection extends BaseInputConnection
	{

		private MyEditable mEditable;

		public MyInputConnection(View targetView, boolean fullEditor)
		{

			super(targetView, fullEditor);

		}

		private class MyEditable extends SpannableStringBuilder
		{

			MyEditable(CharSequence source)
			{
        
				super(source);

			}

			@Override
			public SpannableStringBuilder replace(final int start, final int end, CharSequence tb, int tbstart, int tbend)
			{

				if (tbend > tbstart)
				{

					super.replace(0, length(), "", 0, 0);

					return super.replace(0, 0, tb, tbstart, tbend);

				}
				else if (end > start)
				{
            
					super.replace(0, length(), "", 0, 0);

					return super.replace(0, 0, DUMMY, 0, DUMMY.length());

				}

				return super.replace(start, end, tb, tbstart, tbend);

			}

		}

		@Override
		public Editable getEditable()
		{
    
			if (Build.VERSION.SDK_INT < 14)
				return super.getEditable();

			if (mEditable == null)
			{
        
				mEditable = this.new MyEditable(DUMMY);

				Selection.setSelection(mEditable, DUMMY.length());

			}
			else if (mEditable.length() == 0)
			{
        
				mEditable.append(DUMMY);

				Selection.setSelection(mEditable, DUMMY.length());

			}

			return mEditable;

		}


		@Override
		public boolean deleteSurroundingText(int beforeLength, int afterLength)
		{
    
			// Not called in latest Android version...

			return super.deleteSurroundingText(beforeLength, afterLength);

		}

	}



    private Bitmap m_bitmap;

    private long m_lStartTime;

	private int m_iScreenW;

	private int m_iScreenH;

	private TakeInfoResult m_result;

    private static native void renderImpact(Bitmap  bitmap, long time_ms, TakeInfoResult result);

	private static native void lButtonDown(float x, float y);

	private static native void mouseMove(float x, float y);

	private static native void lButtonUp(float x, float y);

	private static native void keyDown(int keycode);

	private static native void keyUp(int keycode);

	private static native void keyPreImeDown(int keycode, int iUni);

	private static native void keyPreImeUp(int keycode, int iUni);

	private static native void onReceivedShowKeyboard();

	private static native void onReceivedHideKeyboard();

	private static native void onText(String str);

	private static native void audioParameters(int iSampleRate, int iBufferSize);

	private String DUMMY;


    public view(Context context, Point size)
	{

        super(context);

		AudioManager audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
		String frameRate = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
		int frameRateInt = Integer.parseInt(frameRate); // Convert to int
		if (frameRateInt == 0) frameRateInt = -1;// = 44100 // Use a default value if property not found

		String framesPerBuffer = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
		int framesPerBufferInt = Integer.parseInt(framesPerBuffer); // Convert to int
		if (framesPerBufferInt == 0) framesPerBufferInt = -1; //= 256; // Use default
		
		audioParameters(frameRateInt, framesPerBufferInt);


		m_result = new TakeInfoResult();

		//m_result.m_strOpenUrl = new String();

		setFocusableInTouchMode(true);

//		setOnKeyListener(new OnKeyListener()
//			{

//				public boolean onKey(View v, int keyCode, KeyEvent event)
//				{

//	                if (event.getAction() == KeyEvent.ACTION_DOWN)
//					{

//						keyDown(keyCode);

//	                    return true;

//		            }
//	                else if (event.getAction() == KeyEvent.ACTION_UP)
//					{

//						keyUp(keyCode);

//	                    return true;

//		            }

//		            return false;
	
//				}

//            });

        m_iScreenW = size.x;

        m_iScreenH = size.y;

        m_bitmap = Bitmap.createBitmap(m_iScreenW, m_iScreenH - 66, Bitmap.Config.ARGB_8888);

        m_lStartTime = System.currentTimeMillis();

        this.setOnKeyListener(this);

        // Generate a dummy buffer string
        // Make longer or shorter as desired.
        DUMMY = "";
        for (int i = 0; i < 1000; i++)
            DUMMY += "\0";

    }





    @Override
	protected void onDraw(Canvas canvas)
	{
        
		renderImpact(m_bitmap, System.currentTimeMillis() - m_lStartTime, m_result);

        canvas.drawBitmap(m_bitmap, 0, 0, null);

        invalidate();

		if(m_result.m_bShowKeyboard)
		{

			m_result.m_bShowKeyboard = false;

			onReceivedShowKeyboard();

			InputMethodManager imm = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				
			imm.showSoftInput(this, InputMethodManager.SHOW_FORCED);

		}


		if(m_result.m_bHideKeyboard)
		{

			Log.d("com.ca2.app.view", "onDraw Start Hiding Soft Keyboard");

			m_result.m_bHideKeyboard = false;

			onReceivedHideKeyboard();

			InputMethodManager imm = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				
//			imm.hideSoftInputFromWindow (getWindowToken(), InputMethodManager.HIDE_IMPLICIT_ONLY);

			imm.hideSoftInputFromWindow (getWindowToken(), 0);

			Log.d("com.ca2.app.view", "onDraw Soft Keyboard (Should be) Hidden (2)");

		}


		if(m_result.m_strOpenUrl != null && m_result.m_strOpenUrl.length() > 0)
		{

			openUrl(m_result.m_strOpenUrl);

			m_result.m_strOpenUrl = "";

		}

		if(m_result.m_strWallpaper != null && m_result.m_strWallpaper.length() > 0)
		{

			setUserWallpaper(m_result.m_strWallpaper);

			m_result.m_strWallpaper = "";

		}

    }


    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs)
	{

        MyInputConnection ic = new MyInputConnection(this, false);

        outAttrs.inputType = InputType.TYPE_NULL;

		//outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;

        return ic;

    }


    @Override
    public boolean onKey(View view, int keyCode, KeyEvent keyEvent)
	{

        int action = keyEvent.getAction();

        // Catch unicode characters (even character sequeneces)
        // But make sure we aren't catching the dummy buffer.
        if (action == KeyEvent.ACTION_MULTIPLE)
		{

            String s = keyEvent.getCharacters();

            if (!s.equals(DUMMY) && !s.equals("\n")) 
			{
            
			    onText(s);

            }

        }

        // Catch key presses...
        else if (action == KeyEvent.ACTION_DOWN)
		{

			keyPreImeDown(keyCode, keyEvent.getUnicodeChar());

//            switch (keyCode) {
//                case KeyEvent.KEYCODE_DEL:
//                    ...
//                    break;
//                case KeyEvent.KEYCODE_ENTER:
//                    ...
//                    break;
//                case KeyEvent.KEYCODE_TAB:
//                    ...
//                    break;
//                default:
//                   char ch = (char)keyEvent.getUnicodeChar();
//                    if (ch != '\0') {
//                        ...
//                   }
//                    break;
//            }
        }
        else if (action == KeyEvent.ACTION_UP)
		{

			keyPreImeUp(keyCode, keyEvent.getUnicodeChar());

		}

        return false;
    }


//	@Override
//	public InputConnection onCreateInputConnection(EditorInfo outAttrs)
//	{
    
//		outAttrs.actionLabel = null;

//		outAttrs.label = "Test text";

//		outAttrs.inputType = InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;

//		outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;

//		return new inputDislexia(this, true);

//	}


//	@Override
//	public boolean onCheckIsTextEditor()
//	{
    
//		return true;

//	}


//   @Override
//   public InputConnection onCreateInputConnection(EditorInfo outAttrs)
//   {
//		BaseInputConnection fic = new BaseInputConnection(this, true); // avoid send raw events (avoid sending key up key down)
//		BaseInputConnection fic = new BaseInputConnection(this, false); // send raw events (key up key down)
//		outAttrs.actionLabel = null;
//      outAttrs.inputType = InputType.TYPE_CLASS_TEXT; // if is rich editable (offer suggestions?)
//		outAttrs.inputType = InputType.TYPE_NULL; // send raw events
//		outAttrs.imeOptions = EditorInfo.IME_ACTION_NEXT;
//		outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;
//      return fic;
//	}

//  @Override
//  public boolean onCheckIsTextEditor() 
//	{
//		return true;
//  }

	public boolean onTouchEvent (final MotionEvent ev)
	{
        // my Code
        //boolean b = super.onTouchEvent(ev);

		if(ev.getAction() == MotionEvent.ACTION_DOWN)
		{

			lButtonDown(ev.getX(), ev.getY());
		
		}
		else if(ev.getAction() == MotionEvent.ACTION_MOVE)
		{

			mouseMove(ev.getX(), ev.getY());
		
		}
		else if(ev.getAction() == MotionEvent.ACTION_UP)
		{

			lButtonUp(ev.getX(), ev.getY());
		
		}

		//return b;
		return true;
    }


	public boolean onKeyPreIme(int keyCode, KeyEvent ev)
	{

		if(ev.getAction() == KeyEvent.ACTION_DOWN)
		{

			keyPreImeDown(keyCode, ev.getUnicodeChar());
		
		}
		else if(ev.getAction() == KeyEvent.ACTION_UP)
		{

			keyPreImeUp(keyCode, ev.getUnicodeChar());
		
		}
		
		return true;

	}


    private void openUrl( String url )
    {
        
		Uri uriUrl = Uri.parse( url );

        Intent intent = new Intent(Intent.ACTION_VIEW, uriUrl);

        intent.setFlags( Intent.FLAG_ACTIVITY_NEW_TASK );

        getContext().startActivity( intent );

    } 


	private void setUserWallpaper(String path)
	{

		InputStream input = null;

		try
		{

			input = new URL(path).openStream();
    
		}
		catch (IOException e)
		{
			
			e.printStackTrace();

			return;

		}

		if(input == null)
		{

			return;

		}
		
		Bitmap bmp = BitmapFactory.decodeStream(input);

		if(bmp == null)
		{

			return;

		}
    
		BitmapDrawable bitmapDrawable = new BitmapDrawable(bmp);

		if(bitmapDrawable == null)
		{

			return;	

		}
    
		WallpaperManager m = WallpaperManager.getInstance(getContext());

		if(m == null)
		{

			return;

		}

		try
		{
        
			m.setBitmap(bmp);
    
		}
		catch (IOException e)
		{
			
			e.printStackTrace();

		}
		
	}

}



