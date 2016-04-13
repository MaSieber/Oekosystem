using UnityEngine;
using System.Collections;

public class EnergySuckout : MonoBehaviour {

    enum Events
    {
        None = 0,
        EnergySuckOut = 1,
    }
    private Events eventType;
    private bool bIsStable;

    // Use this for initialization
    void Start ()
    {
        eventType = Events.None;
	}
	
	// Update is called once per frame
	void Update ()
    {
        switch (eventType)
        {
            case Events.EnergySuckOut:

                int childCount = transform.childCount;
                for (int i=0; i < childCount; i++)
                {
                    GameObject child = transform.GetChild(i).gameObject;
                    EnergySuckoutBox suckoutBox = child.GetComponent<EnergySuckoutBox>();

                    float rndX = Random.Range(-1.0f, 1.0f);
                    float rndY = Random.Range(2.0f, 5.0f);
                    suckoutBox.startDirection = new Vector3(rndX, rndY, 0.0f);
                    suckoutBox.bDestroying = true;
                }

                eventType = Events.None;
                break;
        }
	}

    void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.name == "Player")
        {
            this.gameObject.GetComponent<BoxCollider2D>().enabled = false;
            eventType = Events.EnergySuckOut;
            collision.gameObject.SendMessage("msg_energySteal", 4);
        }
    }
}
